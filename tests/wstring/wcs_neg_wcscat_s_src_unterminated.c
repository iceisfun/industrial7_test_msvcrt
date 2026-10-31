/* TC
 * id:    wcs_neg_wcscat_s_src_unterminated
 * area:  wstring
 * funcs:
 * procs: wcscat_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs strcat_s, wcscat_s: ERANGE when dest is too small (dest[0] set to 0, handler invoked); EINVAL for NULL
 * note:  src does not fit in the remaining room of dest (despite the id, src IS terminated). ERANGE+handler is documented; dest[0]=0
 *        after failure is well-known behaviour but not clearly documented for strcat_s, hence observed. NULL src: EINVAL+handler.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, wcscat_s, (wchar_t *, size_t, const wchar_t *));
static const wchar_t *volatile ps = L"cdefgh";
static const wchar_t *volatile pn = NULL;
#define PRE() do { if (!tc_p_wcscat_s) TC_SKIP("not exported"); tc_iph_count = 0; errno = 0; } while (0)

TC_CASE(src_unterminated_erange)
{
    wchar_t d[6] = { L'a', L'b', 0, 0x5555, 0x5555, 0x5555 }; int r; PRE();
    r = tc_p_wcscat_s(d, 6, ps);   /* needs 2+6+1 = 9 */
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(d[0], 0);
}
TC_CASE(dest_empty_src_too_long)
{
    wchar_t d[3] = { 0, 0x5555, 0x5555 }; int r; PRE();
    r = tc_p_wcscat_s(d, 3, ps);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(d[0], 0);
}
TC_CASE(exact_fit_control)
{
    wchar_t d[6] = { L'a', L'b', 0, 0x5555, 0x5555, 0x5555 }; int r; PRE();
    r = tc_p_wcscat_s(d, 6, L"cde");
    CHECK_EQ_I(r, 0); CHECK_WSTR(d, L"abcde"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(src_null)
{
    wchar_t d[6] = { L'a', L'b', 0, 0, 0, 0 }; int r; PRE();
    r = tc_p_wcscat_s(d, 6, pn);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(wcscat_s);
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(src_unterminated_erange); TC_RUN(dest_empty_src_too_long); TC_RUN(exact_fit_control); TC_RUN(src_null);
    return tc_end();
}
