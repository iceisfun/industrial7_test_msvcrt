/* TC
 * id:    m99_trunc_basic
 * area:  math_c99
 * funcs: trunc truncf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.9.8; MS docs trunc
 */
#include "tc.h"
#include <math.h>

TC_CASE(pos_neg) { volatile double a = 2.7, b = -2.7; CHECK_BITS_D(trunc(a), 2.0); CHECK_BITS_D(trunc(b), -2.0); }
TC_CASE(neg_half_is_neg_zero) { volatile double a = -0.5; CHECK_BITS_D(trunc(a), -0.0); }
TC_CASE(large_unchanged) { volatile double a = 4503599627370497.0, b = 1e300; CHECK_BITS_D(trunc(a), 4503599627370497.0); CHECK_BITS_D(trunc(b), 1e300); }
TC_CASE(floats) { volatile float a = -3.9f, b = 0.9f; CHECK_BITS_F(truncf(a), -3.0f); CHECK_BITS_F(truncf(b), 0.0f); }

int main(void) { tc_begin(); TC_RUN(pos_neg); TC_RUN(neg_half_is_neg_zero); TC_RUN(large_unchanged); TC_RUN(floats); return tc_end(); }
