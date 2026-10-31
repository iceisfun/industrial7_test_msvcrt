/* TC
 * id:    cv_wcstol_wide_digits
 * area:  conv
 * funcs: wcstol wcstoul
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.29.4.1.2 (only the basic digits 0-9 and letters are digits; iswspace set)
 * note:  PROBE: Arabic-Indic digits (U+0661..) are not valid digits for wcstol in the C locale (expected by C standard: only basic digits). Behaviour for fullwidth digits and non-ASCII spaces is probed in cv_wcstol_wide_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>

TC_CASE(arabic_indic_not_digit)
{
    const wchar_t *volatile s = L"\x0661\x0662"; wchar_t *e = NULL;
    CHECK_EQ_I(wcstol(s, &e, 10), 0); CHECK_EQ_P(e, s);
    e = NULL; CHECK_EQ_U(wcstoul(s, &e, 10), 0); CHECK_EQ_P(e, s);
}
TC_CASE(stops_at_arabic_indic)
{
    const wchar_t *volatile s = L"12\x0663"; wchar_t *e = NULL;
    CHECK_EQ_I(wcstol(s, &e, 10), 12); CHECK_EQ_P(e, s + 2);
}

int main(void) { tc_begin(); TC_RUN(arabic_indic_not_digit); TC_RUN(stops_at_arabic_indic); return tc_end(); }
