/* TC
 * id:    m99_neg_cbrt_neg_not_domain_errno_probe
 * area:  math_c99
 * funcs: cbrt
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs cbrt (no errno documented)
 * note:  PROBE: errno stays 0 for cbrt of negative, infinite, NaN and DBL_MAX inputs.
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


TC_CASE(neg) { volatile double x = -27.0; errno = 0; (void)cbrt(x); CHECK_EQ_I(errno, 0); }
TC_CASE(inf) { volatile double i = d_inf(); errno = 0; (void)cbrt(i); (void)cbrt(-i); CHECK_EQ_I(errno, 0); }
TC_CASE(nan) { volatile double n = d_nan(); errno = 0; (void)cbrt(n); CHECK_EQ_I(errno, 0); }
TC_CASE(max) { volatile double m = DBL_MAX; errno = 0; (void)cbrt(m); CHECK_EQ_I(errno, 0); }
int main(void) { tc_begin(); TC_RUN(neg); TC_RUN(inf); TC_RUN(nan); TC_RUN(max); return tc_end(); }
