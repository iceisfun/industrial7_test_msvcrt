/* TC
 * id:    m99_acosh_neg_domain
 * area:  math_c99
 * funcs: acosh acoshf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.2.1 (x<1 is a domain error, returns NaN); MS docs acosh

 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
static double d_inf(void) { return v_one / v_zero; }
static double d_nan(void) { return d_inf() - d_inf(); }
static float f_inf(void) { return f_one / f_zero; }

TC_CASE(below_one) { volatile double a = 0.5, b = -1.0; CHECK_IS_NAN(acosh(a)); CHECK_IS_NAN(acosh(v_zero)); CHECK_IS_NAN(acosh(b)); }
TC_CASE(neg_inf) { CHECK_IS_NAN(acosh(-d_inf())); }
TC_CASE(float_below) { volatile float x = 0.999f; float r = acoshf(x); CHECK(r != r); }
TC_CASE(just_below_one) { volatile double x = 0x1.fffffffffffffp-1; /* 1 - 2^-53 */ CHECK_IS_NAN(acosh(x)); }
int main(void) { tc_begin(); TC_RUN(below_one); TC_RUN(neg_inf); TC_RUN(float_below); TC_RUN(just_below_one); return tc_end(); }
