/* TC
 * id:    mth_sqrt_exact
 * area:  math_basic
 * funcs:
 * procs: sqrt
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.7.5, F.10.4.5; IEEE 754 correctly rounded sqrt
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(double, sqrt, (double));
#define sqrt tc_p_sqrt

static volatile double v_zero = 0.0, v_one = 1.0, v_inf, v_nan;
static void init(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

TC_CASE(zeros) { volatile double z = 0.0, nz = -0.0; CHECK_BITS_D(sqrt(z), 0.0); CHECK_BITS_D(sqrt(nz), -0.0); }
TC_CASE(exact) { volatile double a = 4.0, b = 2.25; CHECK_BITS_D(sqrt(a), 2.0); CHECK_BITS_D(sqrt(b), 1.5); }
TC_CASE(inf) { volatile double a = 1e300, b = 1e300; volatile double i = a * b; CHECK(i == v_inf); CHECK(sqrt(i) == v_inf); }
TC_CASE(two) { volatile double a = 2.0; CHECK_EQ_U(tc_dbits(sqrt(a)), 0x3FF6A09E667F3BCDULL); }
TC_CASE(denorm_min) { volatile double a = 4.9406564584124654e-324; CHECK_NEAR(sqrt(a) / 2.2227587494850775e-162, 1.0, 1e-14); }

int main(void) { tc_begin(); TC_LOAD_PROC(sqrt); init(); TC_RUN(zeros); TC_RUN(exact); TC_RUN(inf); TC_RUN(two); TC_RUN(denorm_min); return tc_end(); }
