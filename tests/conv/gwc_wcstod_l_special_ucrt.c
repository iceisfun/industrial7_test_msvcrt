/* TC
 * id:    gwc_wcstod_l_special_ucrt
 * area:  conv
 * funcs: _wcstod_l _create_locale _free_locale
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   C11 7.22.1.3; MS docs strtod, wcstod (INF, INFINITY, NAN, hex floats in UCRT)
 * note:  msvcr120 omitted (hex float support unverified), see gwc_probe_wcstod_l_special_legacy
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <wchar.h>

static _locale_t L;
static const wchar_t *volatile vp;
static double D(const wchar_t *s, wchar_t **e) { vp = s; return _wcstod_l(vp, e, L); }

static int ispinf(double d) { return d > 1.7976931348623157e308; }
static int isninf(double d) { return d < -1.7976931348623157e308; }
TC_CASE(inf) { const wchar_t *a = L"inf", *b = L"INF", *c = L"Infinity"; wchar_t *e; CHECK(ispinf(D(a, &e))); CHECK_EQ_P(e, a + 3); CHECK(ispinf(D(b, &e))); CHECK_EQ_P(e, b + 3); CHECK(ispinf(D(c, &e))); CHECK_EQ_P(e, c + 8); }
TC_CASE(neg_inf) { const wchar_t *a = L"-inf"; wchar_t *e; CHECK(isninf(D(a, &e))); CHECK_EQ_P(e, a + 4); }
TC_CASE(nan) { const wchar_t *a = L"nan"; wchar_t *e; CHECK_IS_NAN(D(a, &e)); CHECK_EQ_P(e, a + 3); }
TC_CASE(hex_float) { const wchar_t *a = L"0x1p4", *b = L"0x1.8p1"; wchar_t *e; CHECK_BITS_D(D(a, &e), 16.0); CHECK_EQ_P(e, a + 5); CHECK_BITS_D(D(b, &e), 3.0); CHECK_EQ_P(e, b + 7); }
int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(inf); TC_RUN(neg_inf); TC_RUN(nan); TC_RUN(hex_float);
    _free_locale(L);
    return tc_end();
}
