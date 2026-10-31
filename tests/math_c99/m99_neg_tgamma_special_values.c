/* TC
 * id:    m99_neg_tgamma_special_values
 * area:  math_c99
 * funcs: tgamma tgammaf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.8.4, F.10.5.4; MS docs tgamma
 * note:  tgamma(+-0) = +-inf (pole); huge negative integer-valued args are in m99_neg_gamma_huge_negative_probe. Gamma(171.5) ~ 9.5e307 is finite; Gamma overflows past 171.6243. errno asserted in {0,ERANGE,EDOM}.
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
#define DINF (v_one / v_zero)
#define DNAN (v_zero / v_zero)
#define ERR_OK() (errno == 0 || errno == ERANGE || errno == EDOM)

TC_CASE(zero_poles)
{
    volatile double pz = 0.0, nz = -0.0, inf = DINF;
    errno = 0; CHECK_BITS_D(tgamma(pz), inf); CHECK(ERR_OK());
    errno = 0; CHECK_BITS_D(tgamma(nz), -inf); CHECK(ERR_OK());
}
TC_CASE(float_zero_poles)
{
    volatile float pz = 0.0f, nz = -0.0f;
    errno = 0; CHECK_EQ_U(tc_fbits(tgammaf(pz)), 0x7f800000u); CHECK(ERR_OK());
    errno = 0; CHECK_EQ_U(tc_fbits(tgammaf(nz)), 0xff800000u); CHECK(ERR_OK());
}
TC_CASE(inf_nan)
{
    volatile double inf = DINF, n = DNAN; double r;
    errno = 0; CHECK_BITS_D(tgamma(inf), inf); CHECK(ERR_OK());
    r = tgamma(n); CHECK(r != r);
}
TC_CASE(tiny_args)
{
    volatile double a = 1e-300, b = -1e-300;
    CHECK_NEAR(tgamma(a) / 1e300, 1.0, 1e-12);
    CHECK_NEAR(tgamma(b) / 1e300, -1.0, 1e-12);
}
TC_CASE(negative_half_integers)
{
    volatile double a = -0.5, b = -1.5;
    CHECK_NEAR(tgamma(a), -3.5449077018110321, 1e-12);
    CHECK_NEAR(tgamma(b), 2.3632718012073547, 1e-12);
}
TC_CASE(range_edges)
{
    volatile double a = 171.5, b = 171.7, c = -170.5; double r;
    errno = 0; r = tgamma(a); CHECK(r == r && r < DBL_MAX && r > 1e307); CHECK(ERR_OK());
    errno = 0; CHECK_BITS_D(tgamma(b), DINF); CHECK(ERR_OK());
    errno = 0; r = tgamma(c); CHECK(r == r); CHECK(r > -1e-300 && r < 1e-300); CHECK(ERR_OK());
}
int main(void)
{
    tc_begin();
    TC_RUN(zero_poles); TC_RUN(float_zero_poles); TC_RUN(inf_nan); TC_RUN(tiny_args); TC_RUN(negative_half_integers); TC_RUN(range_edges);
    return tc_end();
}
