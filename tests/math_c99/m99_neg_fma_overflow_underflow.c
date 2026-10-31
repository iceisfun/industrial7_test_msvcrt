/* TC
 * id:    m99_neg_fma_overflow_underflow
 * area:  math_c99
 * funcs: fma fmaf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.13.1, F.10.10.1 (fma computes x*y+z with single rounding; invalid cases return NaN)
 * note:  errno accepted as 0 or ERANGE/EDOM for overflow/invalid.
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


TC_CASE(overflow) { volatile double m=DBL_MAX, two=2.0, mt=-2.0, z=0.0; double r;
    errno=0; r=fma(m,two,z); CHECK_BITS_D(r, HUGE_VAL); CHECK(errno==0||errno==ERANGE);
    errno=0; r=fma(m,mt,z); CHECK_BITS_D(r, -HUGE_VAL); CHECK(errno==0||errno==ERANGE); }
TC_CASE(fused_no_intermediate_overflow) { volatile double m=DBL_MAX, two=2.0, nm=-DBL_MAX; CHECK_BITS_D(fma(m,two,nm), DBL_MAX); }
TC_CASE(nan_results) { volatile double i=d_inf(), n=d_nan(), z=0.0, o=1.0, m1=-1.0;
    CHECK_IS_NAN(fma(i,o,-i)); CHECK_IS_NAN(fma(i,m1,i)); CHECK_IS_NAN(fma(n,z,o)); CHECK_IS_NAN(fma(o,o,n)); CHECK_IS_NAN(fma(z,i,n)); }
TC_CASE(single_rounding) { volatile double a=1.0+0x1p-52, b=1.0-0x1p-52, c=-1.0; CHECK_BITS_D(fma(a,b,c), -0x1p-104); }
TC_CASE(float_cases) { volatile float m=FLT_MAX, two=2.0f, nm=-FLT_MAX, z=0.0f, i=f_inf(), o=1.0f; float r;
    CHECK_BITS_F(fmaf(m,two,nm), FLT_MAX); r=fmaf(i,z,o); CHECK(r!=r); }
TC_CASE(underflow) { volatile double t=0x1p-600, z=0.0, d=0x1p-1074; CHECK_BITS_D(fma(t,t,z), 0.0); CHECK_BITS_D(fma(t,t,d), 0x1p-1074); }
int main(void) { tc_begin(); TC_RUN(overflow); TC_RUN(fused_no_intermediate_overflow); TC_RUN(nan_results); TC_RUN(single_rounding); TC_RUN(float_cases); TC_RUN(underflow); return tc_end(); }
