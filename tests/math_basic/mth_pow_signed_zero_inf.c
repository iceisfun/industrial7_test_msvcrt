/* TC
 * id:    mth_pow_signed_zero_inf
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

TC_CASE(zero_odd) { volatile double z = 0.0, nz = -0.0, three = 3.0; CHECK_BITS_D(pow(z, three), 0.0); CHECK_BITS_D(pow(nz, three), -0.0); }
TC_CASE(negzero_even) { volatile double nz = -0.0, two = 2.0, h = 0.5; CHECK_BITS_D(pow(nz, two), 0.0); CHECK_BITS_D(pow(nz, h), 0.0); }
TC_CASE(base_gt1_inf) { volatile double b = 2.0, i = v_inf; CHECK(pow(b, i) == v_inf); CHECK_BITS_D(pow(b, -i), 0.0); }
TC_CASE(base_lt1_inf) { volatile double b = 0.5, i = v_inf; CHECK_BITS_D(pow(b, i), 0.0); }
TC_CASE(neginf_base) { volatile double i = -v_inf, p = 3.0, n = -3.0; CHECK(pow(i, p) == -v_inf); CHECK_BITS_D(pow(i, n), -0.0); }
TC_CASE(posinf_base) { volatile double i = v_inf, n = -1.0; CHECK_BITS_D(pow(i, n), 0.0); }

int main(void) { tc_begin(); TC_LOAD_PROC(pow); init(); TC_RUN(zero_odd); TC_RUN(negzero_even); TC_RUN(base_gt1_inf); TC_RUN(base_lt1_inf); TC_RUN(neginf_base); TC_RUN(posinf_base); return tc_end(); }
