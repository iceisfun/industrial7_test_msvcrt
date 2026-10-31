/* TC
 * id:    mth_neg_exp_pow_range
 * area:  math_basic
 * funcs:
 * procs: exp pow
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs exp, pow (overflow returns HUGE_VAL/inf, errno ERANGE; pow(0,neg) pole; pow(neg,nonint) EDOM)
 * note:  range/domain error returns and errno
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(double, exp, (double));
#define exp tc_p_exp
TC_DECL_PROC(double, pow, (double, double));
#define pow tc_p_pow

static volatile double v_zero = 0.0, v_one = 1.0, v_inf, v_nan;
static void init(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

TC_CASE(exp_overflow) { volatile double x = 1000.0; errno = 0; CHECK(exp(x) == v_inf); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(exp_underflow_zero) { volatile double x = -1000.0; double r = exp(x); CHECK_BITS_D(r, 0.0); }
TC_CASE(pow_overflow) { volatile double x = 10.0, y = 400.0; errno = 0; CHECK(pow(x, y) == v_inf); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(pow_zero_negexp) { volatile double x = 0.0, y = -1.0; CHECK(pow(x, y) == v_inf); }
TC_CASE(pow_neg_nonint) { volatile double x = -8.0, y = 1.0 / 3.0; errno = 0; CHECK_IS_NAN(pow(x, y)); CHECK_EQ_I(errno, EDOM); }

int main(void) { tc_begin(); TC_LOAD_PROC(exp); TC_LOAD_PROC(pow); init(); TC_RUN(exp_overflow); TC_RUN(exp_underflow_zero); TC_RUN(pow_overflow); TC_RUN(pow_zero_negexp); TC_RUN(pow_neg_nonint); return tc_end(); }
