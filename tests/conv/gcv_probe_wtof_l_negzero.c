/* TC
 * id:    gcv_probe_wtof_l_negzero
 * area:  conv
 * funcs: _wtof_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   probe, MS docs atof
 * note:  PROBE: sign of zero. "-0" is expected to give -0.0 (strtod semantics); "-" (no digits, no conversion) is expected to give +0.0 and not -0.0.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static _locale_t L;
static double F(const wchar_t *volatile s) { return _wtof_l(s, L); }

TC_CASE(negzero) { CHECK_BITS_D(F(L"-0"), -0.0); CHECK_BITS_D(F(L"-0.0e3"), -0.0); }
TC_CASE(lone_sign) { CHECK_BITS_D(F(L"-"), 0.0); }

int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    CHECK(L != NULL);
    if (!L) return tc_end() | 1;
    TC_RUN(negzero); TC_RUN(lone_sign);
    _free_locale(L);
    return tc_end();
}
