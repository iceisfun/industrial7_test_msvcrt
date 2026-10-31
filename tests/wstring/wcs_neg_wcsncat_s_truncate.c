/* TC
 * id:    wcs_neg_wcsncat_s_truncate
 * area:  wstring
 * funcs:
 * procs: wcsncat_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncat_s, wcsncat_s: _TRUNCATE appends as much as fits, result NUL-terminated, returns STRUNCATE (80), no handler
 * note:  _TRUNCATE == (size_t)-1, STRUNCATE == 80.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, wcsncat_s, (wchar_t *, size_t, const wchar_t *, size_t));
#define TRUNC ((size_t)-1)
static const wchar_t *volatile pl = L"cdefg";
#define PRE() do { if (!tc_p_wcsncat_s) TC_SKIP("not exported"); tc_iph_count = 0; errno = 0; } while (0)

TC_CASE(truncate_partial) { wchar_t d[6] = { L'a', L'b', 0, 0x5555, 0x5555, 0x5555 }; int r; PRE();
    r = tc_p_wcsncat_s(d, 6, pl, TRUNC); CHECK_EQ_I(r, 80); CHECK_WSTR(d, L"abcde"); CHECK_EQ_I(d[5], 0); }
TC_CASE(truncate_no_room_for_any) { wchar_t d[3] = { L'a', L'b', 0 }; int r; PRE();
    r = tc_p_wcsncat_s(d, 3, pl, TRUNC); CHECK_EQ_I(r, 80); CHECK_WSTR(d, L"ab"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(truncate_fits_ok) { wchar_t d[8] = { L'a', L'b', 0, 0, 0, 0, 0, 0 }; int r; PRE();
    r = tc_p_wcsncat_s(d, 8, L"cd", TRUNC); CHECK_EQ_I(r, 0); CHECK_WSTR(d, L"abcd"); }
TC_CASE(truncate_no_iph_count) { wchar_t d[6]; PRE();
    d[0] = L'a'; d[1] = 0; tc_p_wcsncat_s(d, 6, pl, TRUNC);
    d[0] = L'a'; d[1] = L'b'; d[2] = L'c'; d[3] = L'd'; d[4] = L'e'; d[5] = 0; tc_p_wcsncat_s(d, 6, pl, TRUNC);
    CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(wcsncat_s);
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(truncate_partial); TC_RUN(truncate_no_room_for_any); TC_RUN(truncate_fits_ok); TC_RUN(truncate_no_iph_count);
    return tc_end();
}
