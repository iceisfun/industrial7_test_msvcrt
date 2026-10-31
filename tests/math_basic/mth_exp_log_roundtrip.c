/* TC
 * id:    mth_exp_log_roundtrip
 * area:  math_basic
 * funcs: exp log log10 pow
 * crt:   msvcrt msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.6; MS docs exp/log/log10/pow
 * note:  msvcr80 excluded: pow is not importable from the mingw msvcr80 import library (build limitation).
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

static const double xs[5] = { 0.5, 1.0, 2.5, 10.0, 123.456 };
TC_CASE(log_exp) { static const double a[5] = { -3.0, 0.5, 1.0, 2.5, 10.0 }; int i; for (i = 0; i < 5; i++) { volatile double x = a[i]; CHECK_NEAR(log(exp(x)), x, 1e-14 * (1.0 + fabs(x))); } }
TC_CASE(exp_log) { int i; for (i = 0; i < 5; i++) { volatile double x = xs[i]; double r = exp(log(x)); CHECK_NEAR(r, x, 1e-14 * x); } }
TC_CASE(pow10_log10) { int i; for (i = 0; i < 5; i++) { volatile double x = xs[i], t = 10.0; double r = pow(t, log10(x)); CHECK_NEAR(r, x, 1e-13 * x); } }
TC_CASE(exp_sum) { volatile double a = 0.7, b = 1.9; double l = exp(a + b), r = exp(a) * exp(b); CHECK_NEAR(l, r, 1e-14 * l); }
int main(void) { tc_begin(); errno = 0; TC_RUN(log_exp); TC_RUN(exp_log); TC_RUN(pow10_log10); TC_RUN(exp_sum); return tc_end(); }
