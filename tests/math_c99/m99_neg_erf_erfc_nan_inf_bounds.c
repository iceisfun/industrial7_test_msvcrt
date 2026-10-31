/* TC
 * id:    m99_neg_erf_erfc_nan_inf_bounds
 * area:  math_c99
 * funcs: erf erfc erff erfcf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.5.1 (erf), F.10.5.2 (erfc); MS docs erf, erfc
 * note:  no error is defined for these functions: errno stays 0 and no invalid-parameter handler call occurs.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
#define DINF (v_one / v_zero)
#define DNAN (v_zero / v_zero)

TC_CASE(erf_saturation)
{
    volatile double inf = DINF, six = 6.0;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    CHECK_BITS_D(erf(inf), 1.0);
    CHECK_BITS_D(erf(-inf), -1.0);
    CHECK_NEAR(erf(six), 1.0, 2.3e-16);
    CHECK_NEAR(erf(-six), -1.0, 2.3e-16);
    CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(nan_and_zero)
{
    volatile double n = DNAN, nz = -0.0; double r;
    r = erf(n); CHECK(r != r);
    r = erfc(n); CHECK(r != r);
    errno = 0;
    CHECK_BITS_D(erf(nz), -0.0);
    CHECK_EQ_I(errno, 0);
}
TC_CASE(erfc_bounds)
{
    volatile double inf = DINF, six = 6.0;
    errno = 0;
    CHECK_BITS_D(erfc(-inf), 2.0);
    CHECK_NEAR(erfc(-six), 2.0, 4.5e-16);
    CHECK_BITS_D(erfc(inf), 0.0);
    CHECK_EQ_I(errno, 0);
}
TC_CASE(erf_tiny)
{
    volatile double t = 1e-300; double r = erf(t);
    CHECK(r != 0.0);
    CHECK_NEAR(r / 1.1283791670955126e-300, 1.0, 1e-12);
}
TC_CASE(float_variants)
{
    volatile float inf = (float)DINF, m10 = -10.0f, n = (float)DNAN; float r;
    errno = 0;
    CHECK_BITS_F(erff(inf), 1.0f);
    CHECK_BITS_F(erff(-inf), -1.0f);
    CHECK_NEAR(erfcf(m10), 2.0, 5e-7);
    CHECK_EQ_I(errno, 0);
    r = erfcf(n); CHECK(r != r); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(erf_saturation); TC_RUN(nan_and_zero); TC_RUN(erfc_bounds); TC_RUN(erf_tiny); TC_RUN(float_variants);
    return tc_end();
}
