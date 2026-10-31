/* TC
 * id:    mth_exact_at_identity_points
 * area:  math_basic
 * funcs: cos cosh exp acos log log10 sqrt
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10
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

TC_CASE(ones) { volatile double z = 0.0; CHECK_BITS_D(cos(z), 1.0); CHECK_BITS_D(cosh(z), 1.0); CHECK_BITS_D(exp(z), 1.0); }
TC_CASE(zeros) { volatile double o = 1.0; CHECK_BITS_D(acos(o), 0.0); CHECK_BITS_D(log(o), 0.0); CHECK_BITS_D(log10(o), 0.0); }
TC_CASE(sqrt_) { volatile double o = 1.0; CHECK_BITS_D(sqrt(o), 1.0); }
int main(void) { tc_begin(); errno = 0; TC_RUN(ones); TC_RUN(zeros); TC_RUN(sqrt_); return tc_end(); }
