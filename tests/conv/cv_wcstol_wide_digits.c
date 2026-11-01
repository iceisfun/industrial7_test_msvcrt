/* TC
 * id:    cv_wcstol_wide_digits
 * area:  conv
 * funcs: wcstol wcstoul
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   C11 7.29.4.1.2 (only the basic digits 0-9 and letters are digits; iswspace set)
 * note:  OBSERVED (Win10 22H2 19045 ucrtbase, run twice): wcstol/wcstoul accept Arabic-Indic digits U+0661.. as decimal digit values
 *        (L"\x0661\x0662" -> 12, endptr consumes both; L"12\x0663" -> 123). deviates from C11 7.29.4.1.2 (only 0-9 are digits in the C locale).
 *        Case names changed to match the observed behaviour. Behaviour of fullwidth digits / spaces: cv_wcstol_wide_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>

TC_CASE(arabic_indic_is_digit)
{   /* deviates from C11 7.29.4.1.2: UCRT wcstol/wcstoul accept Unicode decimal digits (Nd), here U+0661 U+0662 == 1 2 */
    const wchar_t *volatile s = L"\x0661\x0662"; wchar_t *e = NULL;
    CHECK_EQ_I(wcstol(s, &e, 10), 12); CHECK_EQ_P(e, s + 2);
    e = NULL; CHECK_EQ_U(wcstoul(s, &e, 10), 12); CHECK_EQ_P(e, s + 2);
}
TC_CASE(continues_through_arabic_indic)
{   /* deviates from C11 7.29.4.1.2: U+0663 is consumed as digit 3 */
    const wchar_t *volatile s = L"12\x0663"; wchar_t *e = NULL;
    CHECK_EQ_I(wcstol(s, &e, 10), 123); CHECK_EQ_P(e, s + 3);
}

int main(void) { tc_begin(); TC_RUN(arabic_indic_is_digit); TC_RUN(continues_through_arabic_indic); return tc_end(); }
