/* TC
 * id:    gmb_bessel_y_basic
 * area:  math
 * funcs: _y0 _y1 _yn
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _y0 _y1 _yn
 */
#include "tc.h"
#include <math.h>

static volatile double v1 = 1.0, v0 = 0.0;

TC_CASE(values_at_1)
{
    CHECK_NEAR(_y0(v1), 0.08825696421567697, 1e-12);
    CHECK_NEAR(_y1(v1), -0.7812128213002887, 1e-12);
    CHECK_NEAR(_yn(2, v1), -1.650682606816254, 1e-11);
}
TC_CASE(first_zero) { volatile double x = 0.8935769662791676; CHECK_NEAR(_y0(x), 0.0, 1e-9); }
TC_CASE(yn_matches)
{
    volatile double a = 0.5, b = 3.7;
    CHECK_NEAR(_yn(0, a), _y0(a), 1e-13); CHECK_NEAR(_yn(1, a), _y1(a), 1e-13);
    CHECK_NEAR(_yn(0, b), _y0(b), 1e-13); CHECK_NEAR(_yn(1, b), _y1(b), 1e-13);
}
int main(void) { tc_begin(); TC_RUN(values_at_1); TC_RUN(first_zero); TC_RUN(yn_matches); (void)v0; return tc_end(); }
