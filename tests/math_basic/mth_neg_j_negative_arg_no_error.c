/* TC
 * id:    mth_neg_j_negative_arg_no_error
 * area:  math_basic
 * funcs: _j0 _j1 _jn
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   Bessel parity: Jn(-x) = (-1)^n Jn(x); MS docs _j0/_j1/_jn list no domain error for J (only _y0/_y1/_yn)
 * note:  Negative x is valid for J-Bessel: result finite, errno untouched. Parity asserted to 1e-14 absolute (values are O(0.1-0.5)); bit-exactness is not required.
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

TC_CASE(j0) { volatile double p = 2.5, m = -2.5; double a, b; errno = 0; a = _j0(m); b = _j0(p); CHECK_NEAR(a, b, 1e-14); CHECK_EQ_I(errno, 0); CHECK(a == a && a < 1e300 && a > -1e300); }
TC_CASE(j1) { volatile double p = 2.5, m = -2.5; double a, b; errno = 0; a = _j1(m); b = _j1(p); CHECK_NEAR(a, -b, 1e-14); CHECK_EQ_I(errno, 0); CHECK(a == a && a < 1e300 && a > -1e300); }
TC_CASE(jn_odd) { volatile double p = 2.5, m = -2.5; double a, b; errno = 0; a = _jn(3, m); b = _jn(3, p); CHECK_NEAR(a, -b, 1e-14); CHECK_EQ_I(errno, 0); CHECK(a == a && a < 1e300 && a > -1e300); }
TC_CASE(jn_even) { volatile double p = 2.5, m = -2.5; double a, b; errno = 0; a = _jn(2, m); b = _jn(2, p); CHECK_NEAR(a, b, 1e-14); CHECK_EQ_I(errno, 0); CHECK(a == a && a < 1e300 && a > -1e300); }

int main(void) { tc_begin(); TC_RUN(j0); TC_RUN(j1); TC_RUN(jn_odd); TC_RUN(jn_even); return tc_end(); }
