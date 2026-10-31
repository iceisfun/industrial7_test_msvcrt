/* TC
 * id:    gmb_bessel_neg_y_domain
 * area:  math
 * funcs: _y0 _y1 _yn
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _j0, _j1, _jn, _y0, _y1, _yn
 * note:  Asserted: errno==EDOM (documented), result non-finite (docs say HUGE_VAL; NaN/inf both accepted), no invalid-parameter handler call.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

/* MS docs say HUGE_VAL is returned; real CRTs may return NaN or inf: assert only non-finite. NaN-ness is probed in gmb_bessel_edge_probe. */
static int finite_val(double d) { return ((tc_dbits(d) >> 52) & 0x7FF) != 0x7FF; }

TC_CASE(y0_neg) { volatile double x = -1.0; double r; int n0 = tc_iph_count; errno = 0; r = _y0(x); CHECK(!finite_val(r)); CHECK_EQ_I(errno, EDOM); CHECK_EQ_I(tc_iph_count, n0); }
TC_CASE(y1_neg) { volatile double x = -2.5; double r; errno = 0; r = _y1(x); CHECK(!finite_val(r)); CHECK_EQ_I(errno, EDOM); }
TC_CASE(yn_neg) { volatile double x = -0.5; double r; errno = 0; r = _yn(2, x); CHECK(!finite_val(r)); CHECK_EQ_I(errno, EDOM); }
TC_CASE(yn_tiny_neg) { volatile double x = -DBL_MIN; double r; errno = 0; r = _yn(0, x); CHECK(!finite_val(r)); CHECK_EQ_I(errno, EDOM); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(y0_neg); TC_RUN(y1_neg); TC_RUN(yn_neg); TC_RUN(yn_tiny_neg);
    return tc_end();
}
