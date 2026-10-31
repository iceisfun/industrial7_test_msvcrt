/* TC
 * id:    wcs_neg_wcsncpy_s_truncate
 * area:  wstring
 * funcs:
 * procs: wcsncpy_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncpy_s, _strncpy_s_l, wcsncpy_s: _TRUNCATE copies as much as fits and returns STRUNCATE (80) without invoking the handler
 * note:  _TRUNCATE == (size_t)-1, STRUNCATE == 80. Without _TRUNCATE, count >= size with a longer src is ERANGE + handler, dest[0]=0.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, wcsncpy_s, (wchar_t *, size_t, const wchar_t *, size_t));
#define TRUNC ((size_t)-1)
static const wchar_t *volatile pl = L"abcdef";
#define PRE() do { if (!tc_p_wcsncpy_s) TC_SKIP("not exported"); tc_iph_count = 0; errno = 0; } while (0)

TC_CASE(truncate_returns_strunc) { wchar_t d[4]; int r; PRE(); r = tc_p_wcsncpy_s(d, 4, pl, TRUNC);
    CHECK_EQ_I(r, 80); CHECK_WSTR(d, L"abc"); CHECK_EQ_I(d[3], 0); }
TC_CASE(truncate_no_iph) { wchar_t d[4]; PRE(); tc_p_wcsncpy_s(d, 4, pl, TRUNC); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(truncate_exact_fit_ok) { wchar_t d[4]; int r; PRE(); r = tc_p_wcsncpy_s(d, 4, L"abc", TRUNC);
    CHECK_EQ_I(r, 0); CHECK_WSTR(d, L"abc"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(size1_truncate) { wchar_t d[2] = { 0x5555, 0x5555 }; int r; PRE(); r = tc_p_wcsncpy_s(d, 1, pl, TRUNC);
    CHECK_EQ_I(r, 80); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(d[1], 0x5555); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(nontruncate_long_src_erange) { wchar_t d[4]; int r; PRE(); d[0] = L'x'; r = tc_p_wcsncpy_s(d, 4, pl, 6);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(wcsncpy_s);
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(truncate_returns_strunc); TC_RUN(truncate_no_iph); TC_RUN(truncate_exact_fit_ok);
    TC_RUN(size1_truncate); TC_RUN(nontruncate_long_src_erange);
    return tc_end();
}
