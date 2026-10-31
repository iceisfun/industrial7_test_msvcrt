/* TC
 * id:    wcs_neg_probe_wcscpy_s_same_ptr
 * area:  wstring
 * funcs:
 * procs: wcscpy_s wcsncpy_s wcscat_s
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wcscpy_s ("behavior is undefined if the source and destination strings overlap")
 * note:  Probes what a release CRT does when dest == src. Believed: wcscpy_s and wcsncpy_s succeed (0) leaving the
 *        (possibly truncated) string intact, no handler call; wcscat_s(d,n,d) never finds a NUL in src and ends in
 *        ERANGE with dest[0]==0 and one handler call.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

TC_DECL_PROC(errno_t, wcscpy_s, (wchar_t *, size_t, const wchar_t *));
TC_DECL_PROC(errno_t, wcsncpy_s, (wchar_t *, size_t, const wchar_t *, size_t));
TC_DECL_PROC(errno_t, wcscat_s, (wchar_t *, size_t, const wchar_t *));
static wchar_t *volatile vd;

TC_CASE(cpy_same_ptr)
{
    wchar_t d[16] = L"abc"; int r;
    if (!tc_p_wcscpy_s || !tc_iph_install()) TC_SKIP("missing");
    vd = d; tc_iph_count = 0; r = tc_p_wcscpy_s(vd, 16, vd);
    CHECK_EQ_I(r, 0); CHECK_WSTR(d, L"abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(ncpy_same_ptr)
{
    wchar_t d[16] = L"abcdef"; int r;
    if (!tc_p_wcsncpy_s || !tc_iph_install()) TC_SKIP("missing");
    vd = d; tc_iph_count = 0; r = tc_p_wcsncpy_s(vd, 16, vd, 3);
    CHECK_EQ_I(r, 0); CHECK_WSTR(d, L"abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(cat_dest_is_src)
{
    wchar_t d[16] = L"abc"; int r;
    if (!tc_p_wcscat_s || !tc_iph_install()) TC_SKIP("missing");
    vd = d; tc_iph_count = 0; r = tc_p_wcscat_s(vd, 16, vd);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(wcscpy_s); TC_LOAD_PROC(wcsncpy_s); TC_LOAD_PROC(wcscat_s);
    TC_RUN(cpy_same_ptr); TC_RUN(ncpy_same_ptr); TC_RUN(cat_dest_is_src);
    return tc_end();
}
