/* TC
 * id:    mth_neg_ldexp_overflow
 * area:  math_basic
 * funcs:
 * procs: ldexp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs ldexp (overflow returns +/-HUGE_VAL, errno ERANGE)
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
TC_DECL_PROC(double, ldexp, (double, int));
#define ldexp tc_p_ldexp

static volatile double v_zero = 0.0, v_one = 1.0;

TC_CASE(pos_overflow) { volatile double x = 1.0; volatile int e = 1024; double r; errno = 0; r = ldexp(x, e); CHECK(r == v_one / v_zero); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(neg_overflow) { volatile double x = -1.0; volatile int e = 5000; double r; errno = 0; r = ldexp(x, e); CHECK(r == -(v_one / v_zero)); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(huge_neg_exp_zero) { volatile double x = 1.0; volatile int e = -5000; CHECK_BITS_D(ldexp(x, e), 0.0); }

int main(void) { tc_begin(); TC_LOAD_PROC(ldexp); if (!tc_p_ldexp) { tc_write("TC FAIL " TC_TEST_ID "::ldexp not exported\n"); return 1; } TC_RUN(pos_overflow); TC_RUN(neg_overflow); TC_RUN(huge_neg_exp_zero); return tc_end(); }
