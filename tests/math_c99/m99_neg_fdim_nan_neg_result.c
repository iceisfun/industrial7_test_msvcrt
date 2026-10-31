/* TC
 * id:    m99_neg_fdim_nan_neg_result
 * area:  math_c99
 * funcs: fdim fdimf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.12.1, F.10.9.1 (fdim: x-y if x>y, else +0; NaN if an arg is NaN)
 * note:  Negative difference clamps to +0.
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>
#include <limits.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
static double d_inf(void) { return v_one / v_zero; }
static double d_nan(void) { return d_inf() - d_inf(); }
static float f_inf(void) { return f_one / f_zero; }
static float f_nan(void) { return f_inf() - f_inf(); }
static double d_snan(void) { union { unsigned long long u; double d; } x; x.u = 0x7ff0000000000001ULL; return x.d; }


TC_CASE(clamp) { volatile double a=1.0,b=2.0; CHECK_BITS_D(fdim(a,b), 0.0); CHECK_BITS_D(fdim(-b,-a), 0.0); }
TC_CASE(inf_cases) { volatile double i=d_inf(); CHECK_BITS_D(fdim(-i,-i), 0.0); CHECK_BITS_D(fdim(i,i), 0.0);
    CHECK_BITS_D(fdim(i,-i), d_inf()); CHECK_BITS_D(fdim(-i,i), 0.0); }
TC_CASE(nan_cases) { volatile double n=d_nan(), o=1.0; CHECK_IS_NAN(fdim(n,o)); CHECK_IS_NAN(fdim(o,n)); CHECK_IS_NAN(fdim(n,n)); }
TC_CASE(float_cases) { volatile float a=1.0f,b=2.0f,m=FLT_MAX,nm=-FLT_MAX; CHECK_BITS_F(fdimf(a,b), 0.0f); CHECK_BITS_F(fdimf(nm,m), 0.0f); }
TC_CASE(no_overflow_when_less) { volatile double m=DBL_MAX, nm=-DBL_MAX; CHECK_BITS_D(fdim(nm,m), 0.0); }
int main(void) { tc_begin(); TC_RUN(clamp); TC_RUN(inf_cases); TC_RUN(nan_cases); TC_RUN(float_cases); TC_RUN(no_overflow_when_less); return tc_end(); }
