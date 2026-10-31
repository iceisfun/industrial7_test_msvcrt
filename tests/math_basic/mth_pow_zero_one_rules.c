/* TC
 * id:    mth_pow_zero_one_rules
 * area:  math_basic
 * funcs:
 * procs: pow
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.4.4
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(double, pow, (double, double));
#define pow tc_p_pow

static volatile double v_zero = 0.0, v_one = 1.0, v_inf, v_nan;
static void init(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

TC_CASE(nan_to_zero) { volatile double x = v_nan, y = 0.0; CHECK_BITS_D(pow(x, y), 1.0); }
TC_CASE(x_to_neg_zero) { volatile double x = 5.0, y = -0.0; CHECK_BITS_D(pow(x, y), 1.0); }
TC_CASE(one_to_inf) { volatile double x = 1.0, y = v_inf; CHECK_BITS_D(pow(x, y), 1.0); }
TC_CASE(zero_zero) { volatile double x = 0.0, y = 0.0; CHECK_BITS_D(pow(x, y), 1.0); }

int main(void) { tc_begin(); TC_LOAD_PROC(pow); init(); TC_RUN(nan_to_zero); TC_RUN(x_to_neg_zero); TC_RUN(one_to_inf); TC_RUN(zero_zero); return tc_end(); }
