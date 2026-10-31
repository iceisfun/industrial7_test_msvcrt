/* TC
 * id:    gwc_probe_wcstod_l_special_legacy
 * area:  conv
 * funcs: _wcstod_l _create_locale _free_locale
 * crt:   msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs strtod (pre-UCRT: no INF/NAN/hex-float recognition)
 * note:  Probing whether pre-UCRT CRTs lack inf/nan/hex-float parsing in _wcstod_l. msvcr120 omitted (it has inf/nan).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <wchar.h>

static _locale_t L;
static const wchar_t *volatile vp;
static double D(const wchar_t *s, wchar_t **e) { vp = s; return _wcstod_l(vp, e, L); }

TC_CASE(inf) { const wchar_t *s = L"inf"; wchar_t *e; double d = D(s, &e); if (TC_BELOW(120)) { CHECK_BITS_D(d, 0.0); CHECK_EQ_P(e, s); } }
TC_CASE(nan) { const wchar_t *s = L"nan"; wchar_t *e; double d = D(s, &e); if (TC_BELOW(120)) { CHECK_BITS_D(d, 0.0); CHECK_EQ_P(e, s); } }
TC_CASE(hex) { const wchar_t *s = L"0x10"; wchar_t *e; double d = D(s, &e); if (TC_BELOW(120)) { CHECK_BITS_D(d, 0.0); CHECK_EQ_P(e, s + 1); } }
int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(inf); TC_RUN(nan); TC_RUN(hex);
    _free_locale(L);
    return tc_end();
}
