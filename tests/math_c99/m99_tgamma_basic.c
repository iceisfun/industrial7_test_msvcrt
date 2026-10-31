/* TC
 * id:    m99_tgamma_basic
 * area:  math_c99
 * funcs: tgamma tgammaf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.8.4; Gamma(n)=(n-1)!, Gamma(1/2)=sqrt(pi), Gamma(-1/2)=-2 sqrt(pi)
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

TC_CASE(factorials) { volatile double a = 1, b = 2, c = 5, d = 6;
    CHECK_NEAR(tgamma(a), 1.0, 1e-10); CHECK_NEAR(tgamma(b), 1.0, 1e-10); CHECK_NEAR(tgamma(c), 24.0, 1e-10); CHECK_NEAR(tgamma(d), 120.0, 1e-10); }
TC_CASE(half) { volatile double a = 0.5, b = -0.5; CHECK_NEAR(tgamma(a), 1.7724538509055159, 1e-14); CHECK_NEAR(tgamma(b), -3.5449077018110318, 1e-13); }
TC_CASE(large) { volatile double a = 171; double r = tgamma(a); CHECK(r < PINF); CHECK_NEAR(r / 7.257415615307994e306, 1.0, 1e-10); }
TC_CASE(float_five) { volatile float a = 5.0f; CHECK_NEAR(tgammaf(a), 24.0, 1e-4); }

int main(void) { tc_begin(); TC_RUN(factorials); TC_RUN(half); TC_RUN(large); TC_RUN(float_five); return tc_end(); }
