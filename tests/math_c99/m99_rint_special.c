/* TC
 * id:    m99_rint_special
 * area:  math_c99
 * funcs: rint rintf nearbyint nearbyintf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.6.3, F.10.6.4; MS docs rint, nearbyint
 */
#include "tc.h"
#include <math.h>
#include <float.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
static double d_inf(void) { return v_one / v_zero; }
static double d_nan(void) { return d_inf() - d_inf(); }
static float f_inf(void) { return f_one / f_zero; }
static float f_nan(void) { return f_inf() - f_inf(); }

TC_CASE(zeros)
{
    volatile double pz = 0.0, nz = -0.0; volatile float pzf = 0.0f, nzf = -0.0f;
    CHECK_BITS_D(rint(pz), 0.0); CHECK_BITS_D(rint(nz), -0.0);
    CHECK_BITS_D(nearbyint(pz), 0.0); CHECK_BITS_D(nearbyint(nz), -0.0);
    CHECK_BITS_F(rintf(pzf), 0.0f); CHECK_BITS_F(rintf(nzf), -0.0f);
    CHECK_BITS_F(nearbyintf(pzf), 0.0f); CHECK_BITS_F(nearbyintf(nzf), -0.0f);
}
TC_CASE(infs)
{
    volatile double p = d_inf(), n = -d_inf(); volatile float pf = f_inf(), nf = -f_inf();
    CHECK_BITS_D(rint(p), d_inf()); CHECK_BITS_D(rint(n), -d_inf());
    CHECK_BITS_D(nearbyint(p), d_inf()); CHECK_BITS_D(nearbyint(n), -d_inf());
    CHECK_BITS_F(rintf(pf), f_inf()); CHECK_BITS_F(rintf(nf), -f_inf());
    CHECK_BITS_F(nearbyintf(pf), f_inf()); CHECK_BITS_F(nearbyintf(nf), -f_inf());
}
TC_CASE(nans)
{
    volatile double x = d_nan(); volatile float xf = f_nan();
    CHECK_IS_NAN(rint(x)); CHECK_IS_NAN(nearbyint(x));
    CHECK_IS_NAN(rintf(xf)); CHECK_IS_NAN(nearbyintf(xf));
}
TC_CASE(dbl_max_unchanged)
{
    volatile double m = DBL_MAX, nm = -DBL_MAX; volatile float fm = FLT_MAX;
    CHECK_BITS_D(nearbyint(m), DBL_MAX); CHECK_BITS_D(nearbyint(nm), -DBL_MAX);
    CHECK_BITS_D(rint(m), DBL_MAX);
    CHECK_BITS_F(rintf(fm), FLT_MAX); CHECK_BITS_F(nearbyintf(fm), FLT_MAX);
}
int main(void) { tc_begin(); TC_RUN(zeros); TC_RUN(infs); TC_RUN(nans); TC_RUN(dbl_max_unchanged); return tc_end(); }
