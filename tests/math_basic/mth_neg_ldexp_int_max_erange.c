/* TC
 * id:    mth_neg_ldexp_int_max_erange
 * area:  math_basic
 * funcs: ldexp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs ldexp ("On overflow, ldexp returns +/-HUGE_VAL and sets errno to ERANGE"); C11 7.12.6.6
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

TC_CASE(one_max) { volatile double x = 1.0; volatile int e = INT_MAX; double r; errno = 0; r = ldexp(x, e); CHECK_BITS_D(r, pinf()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(neg_one_max) { volatile double x = -1.0; volatile int e = INT_MAX; double r; errno = 0; r = ldexp(x, e); CHECK_BITS_D(r, -pinf()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(true_min_max) { volatile double x = 4.9406564584124654e-324; volatile int e = INT_MAX; double r; errno = 0; r = ldexp(x, e); CHECK_BITS_D(r, pinf()); CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); TC_RUN(one_max); TC_RUN(neg_one_max); TC_RUN(true_min_max); return tc_end(); }
