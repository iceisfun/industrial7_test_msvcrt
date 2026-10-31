/* TC
 * id:    mth_hypot_basic
 * area:  math_basic
 * funcs: _hypot
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _hypot, _hypotf, _hypotl
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;

static double H(double a, double b) { volatile double x = a, y = b; return _hypot(x, y); }
TC_CASE(t345) { CHECK_BITS_D(H(3, 4), 5.0); }
TC_CASE(t51213) { CHECK_BITS_D(H(5, 12), 13.0); }
TC_CASE(negs) { CHECK_BITS_D(H(-3, -4), 5.0); }
TC_CASE(zeros) { CHECK_BITS_D(H(0, 0), 0.0); }
TC_CASE(zero_neg) { CHECK_BITS_D(H(0, -7), 7.0); }
TC_CASE(sqrt2) { CHECK_NEAR(H(1, 1), 1.4142135623730951, 1e-15); }

int main(void) { tc_begin(); TC_RUN(t345); TC_RUN(t51213); TC_RUN(negs); TC_RUN(zeros); TC_RUN(zero_neg); TC_RUN(sqrt2); return tc_end(); }
