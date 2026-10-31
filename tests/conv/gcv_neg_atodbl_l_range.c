/* TC
 * id:    gcv_neg_atodbl_l_range
 * area:  conv
 * funcs: _atodbl_l _atoflt_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _atodbl, _atoflt: return 0, _UNDERFLOW or _OVERFLOW
 * note:  Output value on overflow is not asserted. Underflow cases moved to gcv_neg_atodbl_l_underflow_probe. Pre-existing cv_atodbl_atoflt covers non-_l overflow only.
 */
#include "tc.h"
#include <stdlib.h>
#include <math.h>
#include <locale.h>

static _locale_t loc;
static int AD(_CRT_DOUBLE *d, const char *s) { char b[64]; int i = 0; while (s[i]) { b[i] = s[i]; i++; } b[i] = 0; return _atodbl_l(d, b, loc); }
static int AF(_CRT_FLOAT *f, const char *s) { char b[64]; int i = 0; while (s[i]) { b[i] = s[i]; i++; } b[i] = 0; return _atoflt_l(f, b, loc); }

TC_CASE(dbl_overflow) { _CRT_DOUBLE d; CHECK_EQ_I(AD(&d, "1e999"), _OVERFLOW); }
TC_CASE(flt_overflow) { _CRT_FLOAT f; CHECK_EQ_I(AF(&f, "1e99"), _OVERFLOW); }
TC_CASE(ok_after_failures)
{
    _CRT_DOUBLE d; _CRT_FLOAT f; d.x = -1; f.f = -1;
    CHECK_EQ_I(AD(&d, "2.5"), 0); CHECK_BITS_D(d.x, 2.5);
    CHECK_EQ_I(AF(&f, "0.5"), 0); CHECK_BITS_F(f.f, 0.5f);
}

int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::setup _create_locale\n"); return 1; }
    TC_RUN(dbl_overflow); TC_RUN(flt_overflow); TC_RUN(ok_after_failures);
    _free_locale(loc);
    return tc_end();
}
