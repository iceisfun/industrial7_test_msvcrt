/* TC
 * id:    mth_neg_jn_zero_arg_values
 * area:  math_basic
 * funcs: _jn
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   Bessel series: J0(0)=1, Jn(0)=0 for n>0; MS docs _jn (only errors listed are for _yn domain)
 * note:  x = 0 is valid for J-Bessel: no errno, no invalid-parameter call.
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

TC_CASE(n0) { volatile double x = 0.0; errno = 0; CHECK_BITS_D(_jn(0, x), 1.0); CHECK_EQ_I(errno, 0); }
TC_CASE(n_pos) { volatile double x = 0.0; int n; for (n = 1; n <= 5; n++) { errno = 0; CHECK(_jn(n, x) == 0.0); CHECK_EQ_I(errno, 0); } }

int main(void) { tc_begin(); TC_RUN(n0); TC_RUN(n_pos); return tc_end(); }
