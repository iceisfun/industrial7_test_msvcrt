/* TC
 * id:    math_floor_ceil_special
 * area:  math
 * funcs: floor ceil
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.6.2 / F.10.6.1; MS docs floor/ceil
 */
#include "tc.h"
#include <math.h>

static volatile double v_zero = 0.0, v_inf, v_one = 1.0;

TC_CASE(signed_zero) { CHECK_BITS_D(floor(-0.0), -0.0); CHECK_BITS_D(ceil(-0.0), -0.0); CHECK_BITS_D(floor(v_zero), 0.0); }
TC_CASE(ceil_neg_frac_is_neg_zero) { volatile double x = -0.5; CHECK_BITS_D(ceil(x), -0.0); }
TC_CASE(inf_nan) { v_inf = v_one / v_zero; CHECK(floor(v_inf) == v_inf); CHECK(ceil(-v_inf) == -v_inf); CHECK_IS_NAN(floor(v_inf - v_inf)); }
TC_CASE(basic) { volatile double a = 2.5, b = -2.5; CHECK_BITS_D(floor(a), 2.0); CHECK_BITS_D(ceil(a), 3.0); CHECK_BITS_D(floor(b), -3.0); CHECK_BITS_D(ceil(b), -2.0); }

int main(void) { tc_begin(); TC_RUN(signed_zero); TC_RUN(ceil_neg_frac_is_neg_zero); TC_RUN(inf_nan); TC_RUN(basic); return tc_end(); }
