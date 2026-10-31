/* TC
 * id:    mth_hypot_special
 * area:  math_basic
 * funcs: _hypot
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  inferred
 * ref:   C11 F.10.4.3 hypot; MS docs _hypot
 * note:  documented via C99 hypot; _hypot is the same routine in ucrt
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;

static double H(double a, double b) { volatile double x = a, y = b; return _hypot(x, y); }
TC_CASE(inf_nan) { double inf = v_one / v_zero, n = v_zero / v_zero; CHECK(H(inf, n) == inf); CHECK(H(n, -inf) == inf); }
TC_CASE(neg_inf) { double inf = v_one / v_zero; CHECK(H(-inf, 3) == inf); }
TC_CASE(nan_fin) { CHECK_IS_NAN(H(v_zero / v_zero, 3)); }
TC_CASE(no_spurious_overflow) { CHECK_NEAR(H(1e200, 1e200) / 1e200, 1.4142135623730951, 1e-14); }

int main(void) { tc_begin(); TC_RUN(inf_nan); TC_RUN(neg_inf); TC_RUN(nan_fin); TC_RUN(no_spurious_overflow); return tc_end(); }
