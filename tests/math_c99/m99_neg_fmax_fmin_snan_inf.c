/* TC
 * id:    m99_neg_fmax_fmin_snan_inf
 * area:  math_c99
 * funcs: fmax fmin fmaxf fminf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.12.2-7.12.12.3, F.10.9.2-F.10.9.3 (NaN treated as missing data)
 * note:  NaN must not propagate when the other argument is numeric.
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


TC_CASE(nan_vs_inf) { volatile double n=d_nan(), i=d_inf(); CHECK_BITS_D(fmax(n,-i), -d_inf()); CHECK_BITS_D(fmin(n,i), d_inf());
    CHECK_BITS_D(fmax(-i,n), -d_inf()); CHECK_BITS_D(fmin(i,n), d_inf()); }
TC_CASE(both_nan) { volatile double n=d_nan(); CHECK_IS_NAN(fmax(n,n)); CHECK_IS_NAN(fmin(n,n)); }
TC_CASE(inf_pair) { volatile double i=d_inf(); CHECK_BITS_D(fmax(-i,i), d_inf()); CHECK_BITS_D(fmin(-i,i), -d_inf()); CHECK_BITS_D(fmax(i,-i), d_inf()); CHECK_BITS_D(fmin(i,-i), -d_inf()); }
TC_CASE(float_nan) { volatile float n=f_nan(), o=1.0f; CHECK_BITS_F(fmaxf(n,o), 1.0f); CHECK_BITS_F(fminf(o,n), 1.0f);
    CHECK_BITS_F(fmaxf(o,n), 1.0f); CHECK_BITS_F(fminf(n,o), 1.0f); }
int main(void) { tc_begin(); TC_RUN(nan_vs_inf); TC_RUN(both_nan); TC_RUN(inf_pair); TC_RUN(float_nan); return tc_end(); }
