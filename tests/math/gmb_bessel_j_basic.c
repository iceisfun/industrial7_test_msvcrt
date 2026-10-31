/* TC
 * id:    gmb_bessel_j_basic
 * area:  math
 * funcs: _j0 _j1 _jn
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * kind:  positive
 * conf:  inferred
 * ref:   MS docs _j0 _j1 _jn
 * note:  underscore names are the real exports; values from standard math tables.
 */
#include "tc.h"
#include <math.h>

static volatile double v0 = 0.0, v1 = 1.0;

TC_CASE(at_zero)
{
    CHECK_BITS_D(_j0(v0), 1.0);
    CHECK(_j1(v0) == 0.0);   /* sign of zero not asserted */
    CHECK_BITS_D(_jn(0, v0), 1.0);
    CHECK(_jn(1, v0) == 0.0);
    CHECK(_jn(5, v0) == 0.0);
}
TC_CASE(first_zero) { volatile double x = 2.404825557695773; CHECK_NEAR(_j0(x), 0.0, 1e-9); }
TC_CASE(values_at_1)
{
    CHECK_NEAR(_j0(v1), 0.7651976865579666, 1e-12);
    CHECK_NEAR(_j1(v1), 0.4400505857449335, 1e-12);
    CHECK_NEAR(_jn(2, v1), 0.1149034849319005, 1e-12);
}
TC_CASE(jn_matches_j0_j1)
{
    volatile double a = 0.5, b = 3.7;
    CHECK_NEAR(_jn(0, a), _j0(a), 1e-13);
    CHECK_NEAR(_jn(1, a), _j1(a), 1e-13);
    CHECK_NEAR(_jn(0, b), _j0(b), 1e-13);
    CHECK_NEAR(_jn(1, b), _j1(b), 1e-13);
}
int main(void) { tc_begin(); TC_RUN(at_zero); TC_RUN(first_zero); TC_RUN(values_at_1); TC_RUN(jn_matches_j0_j1); return tc_end(); }
