/* TC
 * id:    cv_wcstol_basic
 * area:  conv
 * funcs: wcstol
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.4.1.2; MS docs strtol, wcstol
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>

static long W(const wchar_t *volatile s, int base) { return wcstol(s, NULL, base); }

TC_CASE(decimal) { CHECK_EQ_I(W(L"123", 10), 123); CHECK_EQ_I(W(L"123", 0), 123); }
TC_CASE(negative) { CHECK_EQ_I(W(L"-456", 10), -456); CHECK_EQ_I(W(L"+7", 10), 7); }
TC_CASE(base16) { CHECK_EQ_I(W(L"0x1F", 16), 31); CHECK_EQ_I(W(L"0x1F", 0), 31); CHECK_EQ_I(W(L"ff", 16), 255); CHECK_EQ_I(W(L"0X1f", 0), 31); }
TC_CASE(base36) { CHECK_EQ_I(W(L"zz", 36), 35 * 36 + 35); CHECK_EQ_I(W(L"Zz", 36), 35 * 36 + 35); }
TC_CASE(octal_auto) { CHECK_EQ_I(W(L"017", 0), 15); CHECK_EQ_I(W(L"017", 10), 17); }
TC_CASE(whitespace) { CHECK_EQ_I(W(L" \t\n\v\f\r  42", 10), 42); }
TC_CASE(endptr_wide_units)
{
    const wchar_t *volatile s = L"  123abc"; wchar_t *e = NULL; long v = wcstol(s, &e, 10);
    CHECK_EQ_I(v, 123); CHECK_EQ_P(e, s + 5); CHECK_EQ_I(e - s, 5);
}
TC_CASE(no_digits)
{
    const wchar_t *volatile s = L"  xyz"; wchar_t *e = NULL; long v = wcstol(s, &e, 10);
    CHECK_EQ_I(v, 0); CHECK_EQ_P(e, s);
    s = L""; v = wcstol(s, &e, 10); CHECK_EQ_I(v, 0); CHECK_EQ_P(e, s);
}

int main(void)
{
    tc_begin();
    TC_RUN(decimal); TC_RUN(negative); TC_RUN(base16); TC_RUN(base36); TC_RUN(octal_auto); TC_RUN(whitespace);
    TC_RUN(endptr_wide_units); TC_RUN(no_digits);
    return tc_end();
}
