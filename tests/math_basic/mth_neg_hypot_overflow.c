/* TC
 * id:    mth_neg_hypot_overflow
 * area:  math_basic
 * funcs: _hypot
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _hypot (overflow returns HUGE_VAL/inf, errno ERANGE)
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double pinf(void) { return v_one / v_zero; }
static void note_errno(const char *what, int e) { char b[24]; tc_i64(e, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write(" errno="); tc_write(b); tc_write("\n"); }

TC_CASE(dbl_max) { volatile double x = DBL_MAX, y = DBL_MAX; double r; errno = 0; r = _hypot(x, y); CHECK_BITS_D(r, pinf()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(big_1_5e308) { volatile double x = 1.5e308, y = 1.5e308; double r; errno = 0; r = _hypot(x, y); CHECK_BITS_D(r, pinf()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(finite_no_errno) { volatile double x = 3, y = 4; errno = 0; CHECK_BITS_D(_hypot(x, y), 5.0); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_RUN(dbl_max); TC_RUN(big_1_5e308); TC_RUN(finite_no_errno); return tc_end(); }
