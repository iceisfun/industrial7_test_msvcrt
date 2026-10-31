/* TC
 * id:    mth_signed_zero_trig_win7_probe
 * area:  math_basic
 * funcs: sin tan asin atan sinh tanh
 * crt:   msvcrt msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p0
 * conf:  probe
 * ref:   C11 F.10
 * note:  probing whether pre-UCRT CRTs preserve the sign of -0.0 for sin/tan/asin/atan/sinh/tanh (C99 Annex F requires it; old CRTs may return +0).
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
