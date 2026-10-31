/* TC
 * id:    pfw_wide_float_basic
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 (f e E g G); MS docs printf Type Field Characters
 * note:  exponent digit count is normalised away here (2 digits UCRT vs 3 digits pre-UCRT); see pfw_wide_exp_digits_*. Rounding ties are avoided on purpose.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

/* collapse leading zeros of an exponent: e+005 / e+05 -> e+5 */
static void norm_exp(wchar_t *s)
{
    wchar_t *p, *q;
    for (p = s; *p; p++) {
        if ((*p == L'e' || *p == L'E') && (p[1] == L'+' || p[1] == L'-')) {
            q = p + 2;
            while (*q == L'0' && q[1]) q++;
            { wchar_t *d = p + 2; while ((*d++ = *q++)) ; }
            return;
        }
    }
}

TC_CASE(f_default_6) { wchar_t b[64]; volatile double v = 3.14159265, w = -2.5; _snwprintf(b, 64, L"%f|%f|%f", v, w, 0.0 * (double)v); CHECK_WSTR(b, L"3.141593|-2.500000|0.000000"); }
TC_CASE(f_precision0_rounding) { wchar_t b[64]; volatile double a = 2.6, c = 2.4, d = -2.6, e = 1.5; _snwprintf(b, 64, L"%.0f|%.0f|%.0f|%.0f", a, c, d, e); CHECK_WSTR(b, L"3|2|-3|2"); }
TC_CASE(g_trim) { wchar_t b[96]; volatile double a = 0.5, c = 100000.0, d = 0.0001, e = 1.0, f = 3.14159265; _snwprintf(b, 96, L"%g|%g|%g|%g|%g", a, c, d, e, f); CHECK_WSTR(b, L"0.5|100000|0.0001|1|3.14159"); }
TC_CASE(g_switch_to_exp)
{
    wchar_t b[64]; volatile double a = 1234567.0, c = 0.00001234, d = 123456.0;
    _snwprintf(b, 64, L"%g", a); norm_exp(b); CHECK_WSTR(b, L"1.23457e+6");
    _snwprintf(b, 64, L"%g", c); norm_exp(b); CHECK_WSTR(b, L"1.234e-5");
    _snwprintf(b, 64, L"%G", a); norm_exp(b); CHECK_WSTR(b, L"1.23457E+6");
    _snwprintf(b, 64, L"%g", d); CHECK_WSTR(b, L"123456");
}
TC_CASE(e_upper)
{
    wchar_t b[64]; volatile double a = 1500.0;
    _snwprintf(b, 64, L"%e", a); norm_exp(b); CHECK_WSTR(b, L"1.500000e+3");
    _snwprintf(b, 64, L"%E", a); norm_exp(b); CHECK_WSTR(b, L"1.500000E+3");
    _snwprintf(b, 64, L"%.2e", a); norm_exp(b); CHECK_WSTR(b, L"1.50e+3");
}
TC_CASE(negative_zero) { wchar_t b[64]; volatile double z = -0.0; _snwprintf(b, 64, L"%f|%.1f", z, z); CHECK_WSTR(b, L"-0.000000|-0.0"); }
TC_CASE(large_f) { wchar_t b[96]; volatile double v = 1e15, w = 1e20; _snwprintf(b, 96, L"%f|%.1f", v, w); CHECK_WSTR(b, L"1000000000000000.000000|100000000000000000000.0"); }

int main(void) { tc_begin(); TC_RUN(f_default_6); TC_RUN(f_precision0_rounding); TC_RUN(g_trim); TC_RUN(g_switch_to_exp); TC_RUN(e_upper); TC_RUN(negative_zero); TC_RUN(large_f); return tc_end(); }
