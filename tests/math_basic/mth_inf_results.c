/* TC
 * id:    mth_inf_results
 * area:  math_basic
 * funcs: exp log log10 sinh cosh atan sqrt tanh
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.3-F.10.4
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

#define HALFPI 1.5707963267948966
TC_CASE(exp_) { volatile double i = pinf(); CHECK_BITS_D(exp(i), pinf()); CHECK_BITS_D(exp(-i), 0.0); }
TC_CASE(logs) { volatile double i = pinf(); CHECK_BITS_D(log(i), pinf()); CHECK_BITS_D(log10(i), pinf()); }
TC_CASE(sinh_) { volatile double i = pinf(); CHECK_BITS_D(sinh(i), pinf()); CHECK_BITS_D(sinh(-i), -pinf()); }
TC_CASE(cosh_) { volatile double i = pinf(); CHECK_BITS_D(cosh(i), pinf()); CHECK_BITS_D(cosh(-i), pinf()); }
TC_CASE(atan_) { volatile double i = pinf(); CHECK_NEAR(atan(i), HALFPI, 1e-15); CHECK_NEAR(atan(-i), -HALFPI, 1e-15); }
TC_CASE(sqrt_) { volatile double i = pinf(); CHECK_BITS_D(sqrt(i), pinf()); }
TC_CASE(tanh_) { volatile double i = pinf(); CHECK_BITS_D(tanh(i), 1.0); CHECK_BITS_D(tanh(-i), -1.0); }
int main(void) { tc_begin(); errno = 0; TC_RUN(exp_); TC_RUN(logs); TC_RUN(sinh_); TC_RUN(cosh_); TC_RUN(atan_); TC_RUN(sqrt_); TC_RUN(tanh_); return tc_end(); }
