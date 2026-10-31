/* TC
 * id:    gwc_probe_wcstod_l_nan_sign
 * area:  conv
 * funcs: _wcstod_l _create_locale _free_locale
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.22.1.3 (sign of NaN is implementation-defined)
 * note:  Probing the sign bit of NaN from "-nan" (expected set) and "nan" (expected clear) in UCRT.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <wchar.h>

static _locale_t L;
static const wchar_t *volatile vp;
static double D(const wchar_t *s, wchar_t **e) { vp = s; return _wcstod_l(vp, e, L); }

TC_CASE(neg_nan_sign) { wchar_t *e; CHECK(tc_dbits(D(L"-nan", &e)) >> 63); CHECK_IS_NAN(D(L"-nan", &e)); }
TC_CASE(pos_nan_sign) { wchar_t *e; CHECK(!(tc_dbits(D(L"nan", &e)) >> 63)); }
int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(neg_nan_sign); TC_RUN(pos_nan_sign);
    _free_locale(L);
    return tc_end();
}
