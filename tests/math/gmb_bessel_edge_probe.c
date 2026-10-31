/* TC
 * id:    gmb_bessel_edge_probe
 * area:  math
 * funcs: _jn _y0 _yn
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _jn (n is order); J_-n = (-1)^n J_n
 * note:  PROBE (one guess per case): jn with negative order n=-1,-2 follows the identity; _y0 of a very large argument (1e10) is small (|y|<1e-4) rather than a TLOSS error; _y0/_yn of a negative argument return NaN (docs say HUGE_VAL, so this may be +-inf instead).
 */
#include "tc.h"
#include <math.h>
TC_CASE(jn_neg_order1) { volatile double x = 1.7; volatile int n = -1; CHECK_NEAR(_jn(n, x), -_jn(1, x), 1e-13); }
TC_CASE(jn_neg_order2) { volatile double x = 1.7; volatile int n = -2; CHECK_NEAR(_jn(n, x), _jn(2, x), 1e-13); }
TC_CASE(y0_large) { volatile double x = 1e10; double r = _y0(x); CHECK(r < 1e-4 && r > -1e-4); }
TC_CASE(y0_neg_is_nan) { volatile double x = -1.0; CHECK_IS_NAN(_y0(x)); }
TC_CASE(yn_neg_is_nan) { volatile double x = -1.0; CHECK_IS_NAN(_yn(2, x)); }
int main(void) { tc_begin(); TC_RUN(jn_neg_order1); TC_RUN(jn_neg_order2); TC_RUN(y0_large); TC_RUN(y0_neg_is_nan); TC_RUN(yn_neg_is_nan); return tc_end(); }
