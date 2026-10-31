/* TC
 * id:    gcv_neg_atodbl_l_underflow_probe
 * area:  conv
 * funcs: _atodbl_l _atoflt_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _atodbl, _atoflt (undocumented parsing details)
 * note:  PROBING: _atodbl_l/_atoflt_l return _UNDERFLOW (and store +0) for values far below the smallest denormal.
 */
#include "tc.h"
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <locale.h>

static _locale_t loc;
static int AD(_CRT_DOUBLE *d, const char *s) { char b[64]; int i = 0; while (s[i]) { b[i] = s[i]; i++; } b[i] = 0; return _atodbl_l(d, b, loc); }
static int AF(_CRT_FLOAT *f, const char *s) { char b[64]; int i = 0; while (s[i]) { b[i] = s[i]; i++; } b[i] = 0; return _atoflt_l(f, b, loc); }

TC_CASE(dbl_underflow) { _CRT_DOUBLE d; d.x = -1; CHECK_EQ_I(AD(&d, "1e-999"), _UNDERFLOW); CHECK_BITS_D(d.x, 0.0); }
TC_CASE(flt_underflow) { _CRT_FLOAT f; f.f = -1; CHECK_EQ_I(AF(&f, "1e-99"), _UNDERFLOW); CHECK_BITS_F(f.f, 0.0f); }
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::setup _create_locale\n"); return 1; }
    TC_RUN(dbl_underflow); TC_RUN(flt_underflow);
    _free_locale(loc);
    return tc_end();
}
