/* TC
 * id:    gwc_neg_wcstod_l_range
 * area:  conv
 * funcs: _wcstod_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   C11 7.22.1.3; MS docs strtod, wcstod, _wcstod_l (overflow: +/-HUGE_VAL and ERANGE; no conversion: 0, endptr = nptr)
 * note:  Underflow errno is probed separately (gwc_neg_probe_wcstod_l_underflow_errno); here only the underflow value (0.0 magnitude) is checked.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <locale.h>
#include <wchar.h>

static _locale_t L;
static const wchar_t *volatile vp;
static double D(const wchar_t *s, wchar_t **e) { vp = s; return _wcstod_l(vp, e, L); }

TC_CASE(overflow_pos)
{
    static const wchar_t s[] = L"1e999"; wchar_t *e = NULL; double r; errno = 0;
    r = D(s, &e);
    CHECK(r == HUGE_VAL); CHECK(r > 0); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 5);
}
TC_CASE(overflow_neg)
{
    static const wchar_t s[] = L"-1e999"; wchar_t *e = NULL; double r; errno = 0;
    r = D(s, &e);
    CHECK(r == -HUGE_VAL); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 6);
}
TC_CASE(underflow_value)
{
    static const wchar_t s[] = L"1e-999"; wchar_t *e = NULL; double r;
    r = D(s, &e);
    CHECK(r == 0.0); CHECK_EQ_P(e, s + 6);
}
TC_CASE(nodigits)
{
    static const wchar_t *const in[] = { L"abc", L"", L".", L"e5", L"-", L"+", L"  x" };
    size_t i;
    for (i = 0; i < sizeof in / sizeof in[0]; i++) {
        wchar_t *e = (wchar_t *)1; double r = D(in[i], &e);
        CHECK(r == 0.0); CHECK_EQ_P(e, in[i]);
    }
}
int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(overflow_pos); TC_RUN(overflow_neg); TC_RUN(underflow_value); TC_RUN(nodigits);
    _free_locale(L);
    return tc_end();
}
