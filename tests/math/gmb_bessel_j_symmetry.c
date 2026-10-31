/* TC
 * id:    gmb_bessel_j_symmetry
 * area:  math
 * funcs: _j0 _j1 _jn
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * kind:  positive
 * conf:  inferred
 * ref:   J_n(-x) = (-1)^n J_n(x); recurrence (mathematical identities; Bessel fns are not ISO C)
 * note:  negative-order case moved to gmb_bessel_edge_probe. Even/odd symmetry for negative x is mathematical, not MS-documented.
 */
#include "tc.h"
#include <math.h>

TC_CASE(j0_even)
{
    static const double xs[] = { 0.3, 1.0, 2.5, 7.25, 20.0 }; int i;
    for (i = 0; i < 5; i++) { volatile double x = xs[i], nx = -xs[i]; CHECK_NEAR(_j0(nx), _j0(x), 1e-15); }
}
TC_CASE(j1_odd)
{
    static const double xs[] = { 0.3, 1.0, 2.5, 7.25, 20.0 }; int i;
    for (i = 0; i < 5; i++) { volatile double x = xs[i], nx = -xs[i]; CHECK_NEAR(_j1(nx), -_j1(x), 1e-15); }
}
TC_CASE(jn_parity)
{
    volatile double x = 2.5, nx = -2.5;
    CHECK_NEAR(_jn(2, nx), _jn(2, x), 1e-14);
    CHECK_NEAR(_jn(3, nx), -_jn(3, x), 1e-14);
}
TC_CASE(recurrence)
{
    volatile double x = 1.5;
    CHECK_NEAR(_jn(2, x), 2.0 / x * _j1(x) - _j0(x), 1e-12);
}
int main(void) { tc_begin(); TC_RUN(j0_even); TC_RUN(j1_odd); TC_RUN(jn_parity); TC_RUN(recurrence); return tc_end(); }
