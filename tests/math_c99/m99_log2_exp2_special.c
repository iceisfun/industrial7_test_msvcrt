/* TC
 * id:    m99_log2_exp2_special
 * area:  math_c99
 * funcs: log2 exp2 log2f exp2f
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.3.2 (exp2), F.10.4.4 (log2)
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
#define PINF (v_one / v_zero)
#define QNAN (PINF - PINF)
#define FPINF (f_one / f_zero)
#define FQNAN (FPINF - FPINF)

TC_CASE(log2_zero) { volatile double p = 0.0, n = -0.0; CHECK_BITS_D(log2(p), -PINF); CHECK_BITS_D(log2(n), -PINF); }
TC_CASE(log2_one) { volatile double a = 1.0; CHECK_BITS_D(log2(a), 0.0); }
TC_CASE(log2_inf) { volatile double i = PINF; CHECK_BITS_D(log2(i), PINF); }
TC_CASE(exp2_inf) { volatile double i = PINF; CHECK_BITS_D(exp2(-i), 0.0); CHECK_BITS_D(exp2(i), PINF); }
TC_CASE(exp2_zero) { volatile double p = 0.0, n = -0.0; CHECK_BITS_D(exp2(p), 1.0); CHECK_BITS_D(exp2(n), 1.0); }
TC_CASE(nan_in) { volatile double n = QNAN; volatile float fn = FQNAN; CHECK_IS_NAN(log2(n)); CHECK_IS_NAN(exp2(n)); CHECK_IS_NAN(log2f(fn)); CHECK_IS_NAN(exp2f(fn)); }
TC_CASE(exp2_extremes) { volatile double a = -1074, b = 1023; CHECK_NEAR(exp2(b) / 0x1p1023, 1.0, 1e-12); (void)a; }
TC_CASE(float_special) { volatile float p = 0.0f, i = FPINF; CHECK_BITS_F(log2f(p), -i); CHECK_BITS_F(log2f(i), i); CHECK_BITS_F(exp2f(-i), 0.0f); CHECK_BITS_F(exp2f(i), i); CHECK_BITS_F(log2f(1.0f * f_one), 0.0f); }

int main(void) { tc_begin(); TC_RUN(log2_zero); TC_RUN(log2_one); TC_RUN(log2_inf); TC_RUN(exp2_inf); TC_RUN(exp2_zero); TC_RUN(nan_in); TC_RUN(exp2_extremes); TC_RUN(float_special); return tc_end(); }
