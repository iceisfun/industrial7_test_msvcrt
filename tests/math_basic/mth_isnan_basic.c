/* TC
 * id:    mth_isnan_basic
 * area:  math_basic
 * funcs: _isnan
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _isnan
 */
#include "tc.h"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <math.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static double qnan(void) { return mk(0x7FF8000000000000ULL); }
static double pinf(void) { return mk(0x7FF0000000000000ULL); }

TC_CASE(qnan_) { volatile double x = qnan(); CHECK(_isnan(x) != 0); }
TC_CASE(neg_nan) { volatile double x = -qnan(); CHECK(_isnan(x) != 0); }
TC_CASE(inf_minus_inf) { volatile double i = pinf(); volatile double r = i - i; CHECK(_isnan(r) != 0); }
TC_CASE(zero_times_inf) { volatile double i = pinf(); volatile double r = v_zero * i; CHECK(_isnan(r) != 0); }
TC_CASE(non_nan) { volatile double a = 1.0, b = pinf(), c = -0.0, d = 4.9e-324; CHECK(_isnan(a) == 0); CHECK(_isnan(b) == 0); CHECK(_isnan(c) == 0); CHECK(_isnan(d) == 0); }
int main(void) { tc_begin(); TC_RUN(qnan_); TC_RUN(neg_nan); TC_RUN(inf_minus_inf); TC_RUN(zero_times_inf); TC_RUN(non_nan); return tc_end(); }
