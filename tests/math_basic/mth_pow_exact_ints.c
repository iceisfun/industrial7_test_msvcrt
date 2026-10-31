/* TC
 * id:    mth_pow_exact_ints
 * area:  math_basic
 * funcs:
 * procs: pow
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.12.7.4, F.10.4.4 (exactness of pow results is only recommended, not required)
 * note:  probing that pow() returns bit-exact results where the result is exactly representable (2^10, 10^2, 2^-1,
 *        (-2)^3, 0.5^2, x^1); expected on any decent pow but not mandated by the standard or MS docs
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(double, pow, (double, double));
#define pow tc_p_pow

static volatile double v_zero = 0.0, v_one = 1.0, v_inf, v_nan;
static void init(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

TC_CASE(pos_int) { volatile double a = 2, b = 10, c = 10, d = 2; CHECK_BITS_D(pow(a, b), 1024.0); CHECK_BITS_D(pow(c, d), 100.0); }
TC_CASE(neg_exp) { volatile double a = 2, b = -1; CHECK_BITS_D(pow(a, b), 0.5); }
TC_CASE(neg_base) { volatile double a = -2, b = 3, c = 2; CHECK_BITS_D(pow(a, b), -8.0); CHECK_BITS_D(pow(a, c), 4.0); }
TC_CASE(frac_base) { volatile double a = 0.5, b = 2; CHECK_BITS_D(pow(a, b), 0.25); }
TC_CASE(exp_one) { volatile double a = 3.14159, b = 1, c = -7.5; CHECK_BITS_D(pow(a, b), 3.14159); CHECK_BITS_D(pow(c, b), -7.5); }
TC_CASE(sqrt2) { volatile double a = 2, b = 0.5; CHECK_NEAR(pow(a, b), 1.4142135623730951, 1e-15); }

int main(void) { tc_begin(); TC_LOAD_PROC(pow); init(); TC_RUN(pos_int); TC_RUN(neg_exp); TC_RUN(neg_base); TC_RUN(frac_base); TC_RUN(exp_one); TC_RUN(sqrt2); return tc_end(); }
