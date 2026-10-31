/* TC
 * id:    mth_neg_bessel_nan_probe
 * area:  math_basic
 * funcs: _j0 _j1 _jn _y0 _y1 _yn
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _j0 etc.; C11 F.10 (NaN in, NaN out)
 * note:  PROBE: Bessel functions given a quiet NaN return NaN. errno afterwards (0 or EDOM) is printed only.
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

TC_CASE(j0) { volatile double x = qnan(); errno = 0; CHECK_IS_NAN(_j0(x)); info_i("j0_errno", errno); }
TC_CASE(j1) { volatile double x = qnan(); errno = 0; CHECK_IS_NAN(_j1(x)); info_i("j1_errno", errno); }
TC_CASE(jn) { volatile double x = qnan(); errno = 0; CHECK_IS_NAN(_jn(0, x)); CHECK_IS_NAN(_jn(3, x)); info_i("jn_errno", errno); }
TC_CASE(y0) { volatile double x = qnan(); errno = 0; CHECK_IS_NAN(_y0(x)); info_i("y0_errno", errno); }
TC_CASE(y1) { volatile double x = qnan(); errno = 0; CHECK_IS_NAN(_y1(x)); info_i("y1_errno", errno); }
TC_CASE(yn) { volatile double x = qnan(); errno = 0; CHECK_IS_NAN(_yn(0, x)); CHECK_IS_NAN(_yn(4, x)); info_i("yn_errno", errno); }

int main(void) { tc_begin(); TC_RUN(j0); TC_RUN(j1); TC_RUN(jn); TC_RUN(y0); TC_RUN(y1); TC_RUN(yn); return tc_end(); }
