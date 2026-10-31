/* TC
 * id:    mth_neg_trig_huge_finite_no_errno
 * area:  math_basic
 * funcs: sin cos tan atan
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.4.6/7.12.4.7/7.12.4.3 (domain error only for infinite x, F.10.1); MS docs sin, cos, tan
 * note:  Huge FINITE arguments are valid (the CRT must reduce them exactly enough to stay in range): no errno, results in range.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0, v_nan_src;
static double pinf(void) { return v_one / v_zero; }
static double qnan(void) { return v_zero / v_zero; }
static int sgn(double d) { return (int)(tc_dbits(d) >> 63); }
static void info_i(const char *what, long long v) { char b[32]; tc_i64(v, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write("="); tc_write(b); tc_write("\n"); }

TC_CASE(sin_max) { volatile double x = DBL_MAX; double r; errno = 0; r = sin(x); CHECK(r >= -1.0 && r <= 1.0); CHECK_EQ_I(errno, 0); }
TC_CASE(cos_max) { volatile double x = DBL_MAX; double r; errno = 0; r = cos(x); CHECK(r >= -1.0 && r <= 1.0); CHECK_EQ_I(errno, 0); }
TC_CASE(tan_max) { volatile double x = DBL_MAX; double r; errno = 0; r = tan(x); CHECK(r == r && r < pinf() && r > -pinf()); CHECK_EQ_I(errno, 0); }
TC_CASE(sin_negmax) { volatile double x = -DBL_MAX; double r; errno = 0; r = sin(x); CHECK(r >= -1.0 && r <= 1.0); CHECK_EQ_I(errno, 0); }
TC_CASE(sin_1e22) { volatile double x = 1e22; double r; errno = 0; r = sin(x); CHECK(r >= -1.0 && r <= 1.0); CHECK_EQ_I(errno, 0); }
TC_CASE(atan_max) { volatile double x = DBL_MAX; errno = 0; CHECK_NEAR(atan(x), 1.5707963267948966, 1e-15); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_RUN(sin_max); TC_RUN(cos_max); TC_RUN(tan_max); TC_RUN(sin_negmax); TC_RUN(sin_1e22); TC_RUN(atan_max); return tc_end(); }
