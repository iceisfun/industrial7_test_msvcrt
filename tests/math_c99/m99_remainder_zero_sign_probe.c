/* TC
 * id:    m99_remainder_zero_sign_probe
 * area:  math_c99
 * funcs: remainder remainderf remquo
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   IEC 60559 5.3.1 (via C11 F.3); MS docs remainder
 * note:  PROBE: when remainder/remquo is exactly zero from a NONZERO finite dividend, the zero carries the sign of x
 *        (IEEE 754 remainder rule). C11 does not state this explicitly and the MS docs are silent.
 */
#include "tc.h"
#include <math.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
static double d_inf(void) { return v_one / v_zero; }
static float f_inf(void) { return f_one / f_zero; }

TC_CASE(zero_result_sign) { volatile double a = -6.0, b = 3.0, c = 6.0, d = -3.0;
    CHECK_BITS_D(remainder(a, b), -0.0); CHECK_BITS_D(remainder(c, b), 0.0); CHECK_BITS_D(remainder(c, d), 0.0); CHECK_BITS_D(remainder(a, d), -0.0); }
TC_CASE(float_zero_result) { volatile float a = -6.0f, b = 3.0f, c = 6.0f;
    CHECK_BITS_F(remainderf(a, b), -0.0f); CHECK_BITS_F(remainderf(c, b), 0.0f); }
TC_CASE(remquo_zero_result) { volatile double a = -10.0, b = 5.0, c = 10.0; int q = 0;
    CHECK_BITS_D(remquo(a, b, &q), -0.0); CHECK_BITS_D(remquo(c, b, &q), 0.0); }
int main(void) { tc_begin(); TC_RUN(zero_result_sign); TC_RUN(float_zero_result); TC_RUN(remquo_zero_result); return tc_end(); }
