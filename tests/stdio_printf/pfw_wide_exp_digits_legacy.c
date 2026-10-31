/* TC
 * id:    pfw_wide_exp_digits_legacy
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   <140
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_output_format / _TWO_DIGIT_EXPONENT: three-digit exponents are the default before VS2015
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(e5) { wchar_t b[64]; volatile double v = 1e5; _snwprintf(b, 64, L"%e", v); CHECK_WSTR(b, L"1.000000e+005"); }
TC_CASE(em5) { wchar_t b[64]; volatile double v = 1e-5; _snwprintf(b, 64, L"%e", v); CHECK_WSTR(b, L"1.000000e-005"); }
TC_CASE(e100) { wchar_t b[64]; volatile double v = 1e100; _snwprintf(b, 64, L"%e", v); CHECK_WSTR(b, L"1.000000e+100"); }
TC_CASE(g_em5) { wchar_t b[64]; volatile double v = 1e-5; _snwprintf(b, 64, L"%g", v); CHECK_WSTR(b, L"1e-005"); }

int main(void) { tc_begin(); TC_RUN(e5); TC_RUN(em5); TC_RUN(e100); TC_RUN(g_em5); return tc_end(); }
