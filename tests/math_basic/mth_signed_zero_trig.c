/* TC
 * id:    mth_signed_zero_trig
 * area:  math_basic
 * funcs: sin tan asin atan sinh tanh
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.1.2/F.10.1.3/F.10.1.5/F.10.2.2/F.10.2.5 (f(-0)=-0 for sin,tan,asin,atan,sinh,tanh)
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

#define NZ (-0.0)
TC_CASE(sin_tan) { volatile double x = NZ; CHECK_BITS_D(sin(x), NZ); CHECK_BITS_D(tan(x), NZ); }
TC_CASE(asin_atan) { volatile double x = NZ; CHECK_BITS_D(asin(x), NZ); CHECK_BITS_D(atan(x), NZ); }
TC_CASE(sinh_tanh) { volatile double x = NZ; CHECK_BITS_D(sinh(x), NZ); CHECK_BITS_D(tanh(x), NZ); }
int main(void) { tc_begin(); errno = 0; TC_RUN(sin_tan); TC_RUN(asin_atan); TC_RUN(sinh_tanh); return tc_end(); }
