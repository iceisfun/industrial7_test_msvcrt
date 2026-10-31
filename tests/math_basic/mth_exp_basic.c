/* TC
 * id:    mth_exp_basic
 * area:  math_basic
 * funcs:
 * procs: exp
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.6.1, F.10.3.1; MS docs exp
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(double, exp, (double));
#define exp tc_p_exp

static volatile double v_zero = 0.0, v_one = 1.0, v_inf, v_nan;
static void init(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

TC_CASE(zero) { volatile double x = 0.0, y = -0.0; CHECK_BITS_D(exp(x), 1.0); CHECK_BITS_D(exp(y), 1.0); }
TC_CASE(one) { volatile double x = 1.0; CHECK_NEAR(exp(x), 2.718281828459045, 1e-14); }
TC_CASE(minus_one) { volatile double x = -1.0; CHECK_NEAR(exp(x), 0.36787944117144233, 1e-15); }
TC_CASE(neg_inf) { volatile double x = -v_inf; CHECK_BITS_D(exp(x), 0.0); }
TC_CASE(pos_inf) { volatile double x = v_inf; CHECK(exp(x) == v_inf); }
TC_CASE(nan) { volatile double x = v_nan; CHECK_IS_NAN(exp(x)); }

int main(void) { tc_begin(); TC_LOAD_PROC(exp); init(); TC_RUN(zero); TC_RUN(one); TC_RUN(minus_one); TC_RUN(neg_inf); TC_RUN(pos_inf); TC_RUN(nan); return tc_end(); }
