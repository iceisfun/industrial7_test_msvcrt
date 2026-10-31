/* TC
 * id:    mth_neg_pow_zero_neginf_probe
 * area:  math_basic
 * funcs:
 * procs: pow
 * crt:   msvcrt msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 F.10.4.4 (pow(+-0, -inf) = +inf)
 * note:  probing the value of pow(+0, -INF) == +INF; the MS docs do not list this case explicitly, so isolated from the documented pow(0, y<0) test.
 */
#include "tc.h"
#include <math.h>

TC_DECL_PROC(double, pow, (double, double));
static volatile double v_zero = 0.0, v_one = 1.0;
static double INF(void) { return v_one / v_zero; }
static double D(double x) { volatile double v = x; return v; }

TC_CASE(minf) { CHECK_BITS_D(tc_p_pow(D(0.0), -INF()), INF()); }

int main(void) { tc_begin(); TC_LOAD_PROC(pow); CHECK(tc_p_pow); TC_RUN(minf); return tc_end(); }
