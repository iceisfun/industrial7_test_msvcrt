/* TC
 * id:    m99_tgamma_special
 * area:  math_c99
 * funcs: tgamma tgammaf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.5.4
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

TC_CASE(zeros) { volatile double p = 0.0, n = -0.0; CHECK_BITS_D(tgamma(p), PINF); CHECK_BITS_D(tgamma(n), -PINF); }
TC_CASE(inf) { volatile double i = PINF; CHECK_BITS_D(tgamma(i), PINF); }
TC_CASE(nan_in) { volatile double n = QNAN; volatile float fn = FQNAN; CHECK_IS_NAN(tgamma(n)); CHECK_IS_NAN(tgammaf(fn)); }
TC_CASE(float_special) { volatile float p = 0.0f, n = -0.0f, i = FPINF; CHECK_BITS_F(tgammaf(p), i); CHECK_BITS_F(tgammaf(n), -i); CHECK_BITS_F(tgammaf(i), i); }

int main(void) { tc_begin(); TC_RUN(zeros); TC_RUN(inf); TC_RUN(nan_in); TC_RUN(float_special); return tc_end(); }
