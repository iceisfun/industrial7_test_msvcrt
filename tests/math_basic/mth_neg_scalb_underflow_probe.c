/* TC
 * id:    mth_neg_scalb_underflow_probe
 * area:  math_basic
 * funcs: _scalb
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _scalb; C11 F.10.3.13 (scalbn semantics)
 * note:  PROBE: _scalb with extreme long exponents (LONG_MIN/LONG_MAX) saturates like scalbn: +-0 / +-inf, preserving sign.
 *        errno on underflow (ERANGE or 0) is only printed, not asserted.
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

TC_CASE(min_pos) { volatile double x = 1.0; volatile long e = LONG_MIN; CHECK_BITS_D(_scalb(x, e), 0.0); }
TC_CASE(min_neg) { volatile double x = -1.0; volatile long e = LONG_MIN; double r = _scalb(x, e); CHECK(r == 0.0); CHECK_EQ_I(sgn(r), 1); }
TC_CASE(max_inf) { volatile double x = 1.0; volatile long e = LONG_MAX; CHECK_BITS_D(_scalb(x, e), pinf()); }
TC_CASE(zero_max) { volatile double x = 0.0; volatile long e = LONG_MAX; double r; errno = 0; r = _scalb(x, e); CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, 0); }
TC_CASE(inf_neg) { volatile double x = pinf(); volatile long e = -5; CHECK_BITS_D(_scalb(x, e), pinf()); }
TC_CASE(nan_in) { volatile double x = qnan(); volatile long e = 3; CHECK_IS_NAN(_scalb(x, e)); }
TC_CASE(errno_info) { volatile double x = 1.0; volatile long e = LONG_MIN; double r; errno = 0; r = _scalb(x, e); (void)r; info_i("errno_underflow", errno); }

int main(void) { tc_begin(); TC_RUN(min_pos); TC_RUN(min_neg); TC_RUN(max_inf); TC_RUN(zero_max); TC_RUN(inf_neg); TC_RUN(nan_in); TC_RUN(errno_info); return tc_end(); }
