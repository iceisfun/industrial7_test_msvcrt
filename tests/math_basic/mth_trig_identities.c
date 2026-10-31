/* TC
 * id:    mth_trig_identities
 * area:  math_basic
 * funcs: sin cos tan atan asin acos
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.4, 7.12.5; MS docs sin/cos/tan
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

static const double args[8] = { 0.1, 0.5, 1.0, 1.3, 2.0, 3.0, -0.7, -2.5 };
TC_CASE(pythagorean) { int i; for (i = 0; i < 8; i++) { volatile double x = args[i]; double s = sin(x), c = cos(x); CHECK_NEAR(s*s + c*c, 1.0, 1e-14); } }
TC_CASE(tan_ratio) { int i; for (i = 0; i < 8; i++) { volatile double x = args[i]; double s = sin(x), c = cos(x); CHECK_NEAR(tan(x), s / c, 1e-13 * (1.0 + fabs(s / c))); } }
TC_CASE(asin_sin) { volatile double x = 0.5; CHECK_NEAR(asin(sin(x)), 0.5, 1e-14); }
TC_CASE(acos_cos) { volatile double x = 0.5; CHECK_NEAR(acos(cos(x)), 0.5, 1e-14); }
TC_CASE(atan_tan) { volatile double x = 0.5; CHECK_NEAR(atan(tan(x)), 0.5, 1e-14); }
TC_CASE(periodic) { int i; for (i = 0; i < 8; i++) { volatile double x = args[i]; volatile double y = x + 6.283185307179586; CHECK_NEAR(sin(y), sin(x), 1e-14); } }
int main(void) { tc_begin(); errno = 0; TC_RUN(pythagorean); TC_RUN(tan_ratio); TC_RUN(asin_sin); TC_RUN(acos_cos); TC_RUN(atan_tan); TC_RUN(periodic); return tc_end(); }
