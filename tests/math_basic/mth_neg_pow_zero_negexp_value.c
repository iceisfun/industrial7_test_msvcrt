/* TC
 * id:    mth_neg_pow_zero_negexp_value
 * area:  math_basic
 * funcs:
 * procs: pow
 * crt:   msvcrt msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs pow (pow(0, y<0) returns INF); C11 F.10.4.4
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(double, pow, (double, double));
static volatile double v_zero = 0.0, v_one = 1.0;
static double INF(void) { return v_one / v_zero; }
static double D(double x) { volatile double v = x; return v; }

TC_CASE(m1) { CHECK_BITS_D(tc_p_pow(D(0.0), D(-1.0)), INF()); }
TC_CASE(m2) { CHECK_BITS_D(tc_p_pow(D(0.0), D(-2.0)), INF()); }

int main(void) { tc_begin(); TC_LOAD_PROC(pow); CHECK(tc_p_pow); TC_RUN(m1); TC_RUN(m2); return tc_end(); }
