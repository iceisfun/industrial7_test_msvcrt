/* TC
 * id:    mth_neg_yn_tiny_overflow_probe
 * area:  math_basic
 * funcs: _y0 _y1 _yn
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   Y-Bessel -> -inf as x -> 0+; MS docs _y0/_y1/_yn
 * note:  PROBE: tiny positive x. Y1(1e-300) ~ -6.4e299 (finite, or -inf if implementation overflows); Yn(5,1e-300) and
 *        Yn(2,TRUE_MIN) overflow to -inf. Sign must be negative; never +inf. errno (ERANGE/EDOM/0) is printed only.
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

TC_CASE(y0_tiny) { volatile double x = 1e-300; double r; errno = 0; r = _y0(x); CHECK(r < 0.0 && r == r); info_i("y0_errno", errno); }
TC_CASE(y1_tiny) { volatile double x = 1e-300; double r; errno = 0; r = _y1(x); CHECK(r < -1e299); info_i("y1_errno", errno); }
TC_CASE(yn5_tiny) { volatile double x = 1e-300; double r; errno = 0; r = _yn(5, x); CHECK_BITS_D(r, -pinf()); info_i("yn5_errno", errno); }
TC_CASE(yn2_true_min) { volatile double x = 4.9406564584124654e-324; double r; errno = 0; r = _yn(2, x); CHECK_BITS_D(r, -pinf()); info_i("yn2_errno", errno); }

int main(void) { tc_begin(); TC_RUN(y0_tiny); TC_RUN(y1_tiny); TC_RUN(yn5_tiny); TC_RUN(yn2_true_min); return tc_end(); }
