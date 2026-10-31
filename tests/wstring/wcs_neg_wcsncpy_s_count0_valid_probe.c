/* TC
 * id:    wcs_neg_wcsncpy_s_count0_valid_probe
 * area:  wstring
 * funcs:
 * procs: wcsncpy_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strncpy_s, wcsncpy_s
 * note:  PROBE: wcsncpy_s(d, size>0, valid_src, 0) is valid, returns 0 and sets d[0]=0 (no handler). wcsncpy_s(d, 0, src, 0)
 *        with non-NULL d is still invalid (size 0): EINVAL + handler.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, wcsncpy_s, (wchar_t *, size_t, const wchar_t *, size_t));
static const wchar_t *volatile ps = L"abc";
#define PRE() do { if (!tc_p_wcsncpy_s) TC_SKIP("not exported"); tc_iph_count = 0; errno = 0; } while (0)

TC_CASE(count0_valid_src_sets_nul) { wchar_t d[4] = { L'x', L'y', L'z', 0 }; int r; PRE(); r = tc_p_wcsncpy_s(d, 4, ps, 0);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(size0_count0_einval) { wchar_t d[4] = { L'x', L'y', L'z', 0 }; int r; PRE(); r = tc_p_wcsncpy_s(d, 0, ps, 0);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(wcsncpy_s);
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(count0_valid_src_sets_nul); TC_RUN(size0_count0_einval);
    return tc_end();
}
