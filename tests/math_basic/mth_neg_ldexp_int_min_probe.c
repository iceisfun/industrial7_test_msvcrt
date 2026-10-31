/* TC
 * id:    mth_neg_ldexp_int_min_probe
 * area:  math_basic
 * funcs: ldexp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.6.6; MS docs ldexp
 * note:  PROBE: ldexp with exp = INT_MIN / INT_MAX (possible integer overflow inside the CRT's exponent arithmetic).
 *        Expect correctly signed 0 for INT_MIN; ldexp(+-0, INT_MAX) = +-0 with errno untouched. errno for underflow is printed only.
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

TC_CASE(min_pos) { volatile double x = 1.0; volatile int e = INT_MIN; CHECK_BITS_D(ldexp(x, e), 0.0); }
TC_CASE(min_neg) { volatile double x = -1.0; volatile int e = INT_MIN; double r = ldexp(x, e); CHECK(r == 0.0); CHECK_EQ_I(sgn(r), 1); }
TC_CASE(zero_max) { volatile double x = 0.0; volatile int e = INT_MAX; double r; errno = 0; r = ldexp(x, e); CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, 0); }
TC_CASE(errno_info) { volatile double x = 1.0; volatile int e = INT_MIN; double r; errno = 0; r = ldexp(x, e); (void)r; info_i("errno_underflow", errno); }

int main(void) { tc_begin(); TC_RUN(min_pos); TC_RUN(min_neg); TC_RUN(zero_max); TC_RUN(errno_info); return tc_end(); }
