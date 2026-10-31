/* TC
 * id:    gwc_probe_wcstod_l_decimal_comma_locale
 * area:  conv
 * funcs: _wcstod_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _strtod_l (locale parameter supplies the decimal point)
 * note:  Probing that _wcstod_l uses the decimal point of the supplied locale object (German: ','). Skipped when no
 *        German locale can be created on the machine. The C-locale object is checked in the opposite direction.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

static _locale_t LC, LD;
static const wchar_t *volatile vp;
static double D(const wchar_t *s, wchar_t **e, _locale_t l) { vp = s; return _wcstod_l(vp, e, l); }

TC_CASE(de_comma) { const wchar_t *s = L"1,5"; wchar_t *e; if (!LD) TC_SKIP("no German locale"); CHECK_BITS_D(D(s, &e, LD), 1.5); CHECK_EQ_P(e, s + 3); }
TC_CASE(de_point) { const wchar_t *s = L"1.5"; wchar_t *e; if (!LD) TC_SKIP("no German locale"); CHECK_BITS_D(D(s, &e, LD), 1.0); CHECK_EQ_P(e, s + 1); }
TC_CASE(c_opposite) { const wchar_t *s = L"1,5"; const wchar_t *t = L"1.5"; wchar_t *e; CHECK_BITS_D(D(s, &e, LC), 1.0); CHECK_EQ_P(e, s + 1); CHECK_BITS_D(D(t, &e, LC), 1.5); CHECK_EQ_P(e, t + 3); }
int main(void)
{
    tc_begin();
    LC = _create_locale(LC_ALL, "C");
    LD = _create_locale(LC_ALL, "de-DE");
    if (!LD) LD = _create_locale(LC_ALL, "German_Germany.1252");
    if (!LD) LD = _create_locale(LC_ALL, "German");
    if (!LC) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(de_comma); TC_RUN(de_point); TC_RUN(c_opposite);
    if (LD) _free_locale(LD);
    _free_locale(LC);
    return tc_end();
}
