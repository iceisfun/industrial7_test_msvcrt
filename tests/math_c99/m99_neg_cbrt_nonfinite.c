/* TC
 * id:    m99_neg_cbrt_nonfinite
 * area:  math_c99
 * funcs: cbrt cbrtf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.4.1 (cbrt(+-0), cbrt(+-inf) return arg; NaN returns NaN); MS docs cbrt
 * note:  Negative arguments are not a domain error for cbrt. errno is not asserted here (see m99_neg_cbrt_neg_not_domain_errno_probe). Exactness of perfect cubes is not guaranteed by C, so CHECK_NEAR.
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


TC_CASE(nan) { volatile double n = d_nan(); double r = cbrt(n); CHECK_IS_NAN(r); }
TC_CASE(inf) { volatile double i = d_inf(); double r = cbrt(i), s = cbrt(-i); CHECK(r == d_inf()); CHECK(s == -d_inf()); CHECK(r == r); CHECK(s == s); }
TC_CASE(neg_zero) { volatile double z = -0.0; CHECK_BITS_D(cbrt(z), -0.0); }
TC_CASE(neg_not_domain) { volatile double x = -8.0; double r; r = cbrt(x); CHECK_NEAR(r, -2.0, 1e-14); CHECK(r == r); }
TC_CASE(float_neg) { volatile float i = f_inf(), x = -27.0f; CHECK(cbrtf(-i) == -f_inf()); CHECK_NEAR(cbrtf(x), -3.0f, 1e-5); }
TC_CASE(subnormal) { volatile double x = 4.9e-324; double r = cbrt(x); CHECK(r > 1e-110); CHECK(r < 1e-105); }
int main(void) { tc_begin(); TC_RUN(nan); TC_RUN(inf); TC_RUN(neg_zero); TC_RUN(neg_not_domain); TC_RUN(float_neg); TC_RUN(subnormal); return tc_end(); }
