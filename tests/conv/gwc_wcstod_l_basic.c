/* TC
 * id:    gwc_wcstod_l_basic
 * area:  conv
 * funcs: _wcstod_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.3; MS docs strtod, wcstod, _strtod_l, _wcstod_l
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <wchar.h>

static _locale_t L;
static const wchar_t *volatile vp;
static double D(const wchar_t *s, wchar_t **e) { vp = s; return _wcstod_l(vp, e, L); }

TC_CASE(simple) { const wchar_t *s = L"1.5"; wchar_t *e; CHECK_BITS_D(D(s, &e), 1.5); CHECK_EQ_P(e, s + 3); }
TC_CASE(neg_zero) { const wchar_t *s = L"-0"; wchar_t *e; CHECK_BITS_D(D(s, &e), -0.0); CHECK_EQ_P(e, s + 2); }
TC_CASE(ws_exp_stop) { const wchar_t *s = L" \t 2.25e2xyz"; wchar_t *e; CHECK_BITS_D(D(s, &e), 225.0); CHECK_EQ_P(e, s + 9); }
TC_CASE(dot_forms) { const wchar_t *a = L".5", *b = L"5."; wchar_t *e; CHECK_BITS_D(D(a, &e), 0.5); CHECK_EQ_P(e, a + 2); CHECK_BITS_D(D(b, &e), 5.0); CHECK_EQ_P(e, b + 2); }
TC_CASE(exp_forms) { wchar_t *e; CHECK_NEAR(D(L"1e+2", &e), 100.0, 1e-9); CHECK_NEAR(D(L"1E-2", &e), 0.01, 1e-15); }
TC_CASE(bare_e) { const wchar_t *s = L"1e"; const wchar_t *t = L"1e+"; wchar_t *e; CHECK_BITS_D(D(s, &e), 1.0); CHECK_EQ_P(e, s + 1); CHECK_BITS_D(D(t, &e), 1.0); CHECK_EQ_P(e, t + 1); }
TC_CASE(no_conversion) { const wchar_t *s = L"  x1"; wchar_t *e; CHECK_BITS_D(D(s, &e), 0.0); CHECK_EQ_P(e, s); }
int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(simple); TC_RUN(neg_zero); TC_RUN(ws_exp_stop); TC_RUN(dot_forms); TC_RUN(exp_forms); TC_RUN(bare_e); TC_RUN(no_conversion);
    _free_locale(L);
    return tc_end();
}
