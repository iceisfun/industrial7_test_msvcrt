/* TC
 * id:    gwc_wcstod_l_limits_ucrt
 * area:  conv
 * funcs: _wcstod_l _create_locale _free_locale
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   C11 7.22.1.3 (correctly rounded within recommended practice); MS docs strtod
 * note:  UCRT's parser is exact; pre-UCRT parsers are not asserted for last-bit accuracy
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <wchar.h>

static _locale_t L;
static const wchar_t *volatile vp;
static double D(const wchar_t *s, wchar_t **e) { vp = s; return _wcstod_l(vp, e, L); }

TC_CASE(dbl_max) { const wchar_t *s = L"1.7976931348623157e308"; wchar_t *e; CHECK_BITS_D(D(s, &e), 1.7976931348623157e308); CHECK_EQ_P(e, s + 22); }
TC_CASE(denorm_min) { const wchar_t *s = L"4.9406564584124654e-324"; wchar_t *e; CHECK_EQ_U(tc_dbits(D(s, &e)), 1); CHECK_EQ_P(e, s + 23); }
TC_CASE(dbl_min) { wchar_t *e; CHECK_EQ_U(tc_dbits(D(L"2.2250738585072014e-308", &e)), 0x0010000000000000ULL); }
int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(dbl_max); TC_RUN(denorm_min); TC_RUN(dbl_min);
    _free_locale(L);
    return tc_end();
}
