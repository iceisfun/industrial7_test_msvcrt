/* TC
 * id:    gcv_atodbl_l_basic
 * area:  conv
 * funcs: _atodbl_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _atodbl, _atodbl_l (return 0, _UNDERFLOW or _OVERFLOW)
 * note:  Uses an explicit "C" locale handle. Whitespace/junk/no-digit/-0.0 behaviour is undocumented: see gcv_probe_atodbl_l_parse.
 */
#include "tc.h"
#include <stdlib.h>
#include <math.h>
#include <locale.h>

static _locale_t loc;
static int AD(_CRT_DOUBLE *d, const char *s) { char b[64]; int i = 0; while (s[i]) { b[i] = s[i]; i++; } b[i] = 0; return _atodbl_l(d, b, loc); }

TC_CASE(simple) { _CRT_DOUBLE d; d.x = -1; CHECK_EQ_I(AD(&d, "1.5"), 0); CHECK_BITS_D(d.x, 1.5); }
TC_CASE(exponent) { _CRT_DOUBLE d; d.x = -1; CHECK_EQ_I(AD(&d, "1e3"), 0); CHECK_BITS_D(d.x, 1000.0); }

int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::setup _create_locale\n"); return 1; }
    TC_RUN(simple); TC_RUN(exponent);
    _free_locale(loc);
    return tc_end();
}
