/* TC
 * id:    m99_log2_neg_domain
 * area:  math_c99
 * funcs: log2 log2f
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.4.4 (log2 of x<0 is NaN + invalid; log2(+-0) is -inf + divbyzero)
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

TC_CASE(negative_nan) { volatile double a = -1.0, i = PINF; volatile float f = -0.5f; CHECK_IS_NAN(log2(a)); CHECK_IS_NAN(log2(-i)); CHECK_IS_NAN(log2f(f)); }
TC_CASE(zero_pole) { volatile double p = 0.0, n = -0.0; double r = log2(p); CHECK(r < 0 && r == -PINF); r = log2(n); CHECK(r < 0 && r == -PINF); }
TC_CASE(tiny_negative) { volatile double a = -4.9406564584124654e-324; CHECK_IS_NAN(log2(a)); }

int main(void) { tc_begin(); TC_RUN(negative_nan); TC_RUN(zero_pole); TC_RUN(tiny_negative); return tc_end(); }
