/* TC
 * id:    m99_round_special
 * area:  math_c99
 * funcs: round roundf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.6.6
 */
#include "tc.h"
#include <math.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;

TC_CASE(zeros_infs) { volatile double i = v_one / v_zero, p = 0.0, n = -0.0;
    CHECK_BITS_D(round(p), 0.0); CHECK_BITS_D(round(n), -0.0); CHECK_BITS_D(round(i), i); CHECK_BITS_D(round(-i), -i); }
TC_CASE(float_zeros_infs) { volatile float i = f_one / f_zero, p = 0.0f, n = -0.0f;
    CHECK_BITS_F(roundf(p), 0.0f); CHECK_BITS_F(roundf(n), -0.0f); CHECK_BITS_F(roundf(i), i); CHECK_BITS_F(roundf(-i), -i); }
TC_CASE(nans) { volatile double i = v_one / v_zero; volatile double n = i - i; volatile float fi = f_one / f_zero; volatile float fn = fi - fi;
    CHECK_IS_NAN(round(n)); CHECK_IS_NAN(roundf(fn)); }
TC_CASE(large) { volatile double a = 4503599627370497.0, m = DBL_MAX; volatile float f = 8388609.0f;
    CHECK_BITS_D(round(a), 4503599627370497.0); CHECK_BITS_D(round(m), DBL_MAX); CHECK_BITS_F(roundf(f), 8388609.0f); }

int main(void) { tc_begin(); TC_RUN(zeros_infs); TC_RUN(float_zeros_infs); TC_RUN(nans); TC_RUN(large); return tc_end(); }
