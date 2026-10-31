/* TC
 * id:    mth_hyperbolic_identities
 * area:  math_basic
 * funcs: sinh cosh tanh exp
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.5; MS docs sinh/cosh/tanh
 */
#include "tc.h"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <math.h>
#include <float.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static double qnan(void) { return mk(0x7FF8000000000000ULL); }
static double pinf(void) { return mk(0x7FF0000000000000ULL); }

TC_CASE(cosh2_sinh2) { static const double a[3] = { 0.5, 1.0, 2.0 }; int i; for (i = 0; i < 3; i++) { volatile double x = a[i]; double c = cosh(x), s = sinh(x); CHECK_NEAR(c*c - s*s, 1.0, 1e-13); } }
TC_CASE(tanh_ratio) { static const double a[4] = { 0.25, 0.5, 1.0, 2.0 }; int i; for (i = 0; i < 4; i++) { volatile double x = a[i]; CHECK_NEAR(tanh(x), sinh(x) / cosh(x), 1e-14); } }
TC_CASE(sinh_exp) { static const double a[4] = { 0.25, 0.5, 1.0, 2.0 }; int i; for (i = 0; i < 4; i++) { volatile double x = a[i]; double e = exp(x), f = exp(-x); CHECK_NEAR(sinh(x), (e - f) / 2.0, 1e-14); } }
int main(void) { tc_begin(); errno = 0; TC_RUN(cosh2_sinh2); TC_RUN(tanh_ratio); TC_RUN(sinh_exp); return tc_end(); }
