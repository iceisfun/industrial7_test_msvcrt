/* TC
 * id:    m99_neg_expm1_log1p_extremes
 * area:  math_c99
 * funcs: expm1 expm1f log1p
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.3.4 (expm1), F.10.3.9 (log1p); MS docs expm1, log1p
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
#define DINF (v_one / v_zero)
#define DNAN (v_zero / v_zero)
#define ERR_OK() (errno == 0 || errno == ERANGE || errno == EDOM)

TC_CASE(expm1_saturates)
{
    volatile double a = -DINF, b = -800.0, c = -1e300;
    CHECK_BITS_D(expm1(a), -1.0);
    CHECK_BITS_D(expm1(b), -1.0);
    CHECK_BITS_D(expm1(c), -1.0);
}
TC_CASE(expm1_nan_inf)
{
    volatile double n = DNAN, inf = DINF; double r;
    r = expm1(n); CHECK(r != r);
    errno = 0; CHECK_BITS_D(expm1(inf), inf); CHECK(ERR_OK());
}
TC_CASE(expm1_tiny)
{
    volatile double t = 1e-300, nz = -0.0;
    CHECK_NEAR(expm1(t) / 1e-300, 1.0, 1e-12);
    CHECK_BITS_D(expm1(nz), -0.0);
}
TC_CASE(expm1f_saturates)
{
    volatile float a = (float)-DINF, b = -100.0f;
    CHECK_BITS_F(expm1f(a), -1.0f);
    CHECK_BITS_F(expm1f(b), -1.0f);
}
TC_CASE(log1p_special)
{
    volatile double inf = DINF, n = DNAN, nz = -0.0, t = 1e-300; double r;
    errno = 0; CHECK_BITS_D(log1p(inf), inf); CHECK(ERR_OK());
    r = log1p(n); CHECK(r != r);
    CHECK_BITS_D(log1p(nz), -0.0);
    CHECK_NEAR(log1p(t) / 1e-300, 1.0, 1e-12);
}
TC_CASE(log1p_near_minus_one)
{
    volatile double x = -0.99999999999999988898;  /* -(1 - 2^-53) */
    double r = log1p(x);
    CHECK(r == r);
    CHECK(r > -1e308 && r < 0.0);
    CHECK_NEAR(r, -36.7368005696771, 1e-9);
}
int main(void)
{
    tc_begin();
    TC_RUN(expm1_saturates); TC_RUN(expm1_nan_inf); TC_RUN(expm1_tiny); TC_RUN(expm1f_saturates); TC_RUN(log1p_special); TC_RUN(log1p_near_minus_one);
    return tc_end();
}
