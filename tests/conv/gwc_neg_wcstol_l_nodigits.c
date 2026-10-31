/* TC
 * id:    gwc_neg_wcstol_l_nodigits
 * area:  conv
 * funcs: _wcstol_l _wcstoul_l _wcstoi64_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.4 (no conversion performed: value 0, endptr = nptr); MS docs wcstol, _wcstol_l
 * note:  errno is not asserted. "0x" with base 16 is covered by gwc_probe_wcstol_l_0x_nodigits. Whitespace-only input leaves endptr at the ORIGINAL nptr.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

static _locale_t L;
static const wchar_t *volatile vp;

#define NODIG(s, base) do { const wchar_t *tc_s = (s); wchar_t *e1 = (wchar_t *)1, *e2 = (wchar_t *)1, *e3 = (wchar_t *)1; \
    vp = tc_s; CHECK_EQ_I(_wcstol_l(vp, &e1, base, L), 0);   CHECK_EQ_P(e1, tc_s); \
    vp = tc_s; CHECK_EQ_U(_wcstoul_l(vp, &e2, base, L), 0);  CHECK_EQ_P(e2, tc_s); \
    vp = tc_s; CHECK_EQ_I(_wcstoi64_l(vp, &e3, base, L), 0); CHECK_EQ_P(e3, tc_s); } while (0)

TC_CASE(empty)      { NODIG(L"", 10); }
TC_CASE(spaces)     { NODIG(L"   ", 10); }
TC_CASE(letters)    { NODIG(L"abc", 10); }
TC_CASE(sign_only)  { NODIG(L"+", 10); NODIG(L"-", 10); }
TC_CASE(double_sign){ NODIG(L"  -+5", 10); }
TC_CASE(digit_out_of_base) { NODIG(L"9", 8); NODIG(L"z", 16); }

int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(empty); TC_RUN(spaces); TC_RUN(letters); TC_RUN(sign_only); TC_RUN(double_sign); TC_RUN(digit_out_of_base);
    _free_locale(L);
    return tc_end();
}
