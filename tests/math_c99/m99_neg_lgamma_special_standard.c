/* TC
 * id:    m99_neg_lgamma_special_standard
 * area:  math_c99
 * funcs: 
 * procs: lgamma lgammaf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.8.3, F.10.5.3; MS docs lgamma
 * note:  lgamma(1) = lgamma(2) = +0. lgamma returns log|Gamma(x)|, so it is finite for negative non-integers. Resolved by name at run time.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

typedef double (__cdecl *pd1)(double); typedef float (__cdecl *pf1)(float);
static pd1 p_lg; static pf1 p_lgf;
static volatile double v_zero = 0.0, v_one = 1.0;
#define DINF (v_one / v_zero)
#define DNAN (v_zero / v_zero)
#define ERR_OK() (errno == 0 || errno == ERANGE || errno == EDOM)

TC_CASE(one_two_plus_zero)
{
    volatile double a = 1.0, b = 2.0;
    if (!p_lg) TC_SKIP("no lgamma");
    CHECK_BITS_D(p_lg(a), 0.0);
    CHECK_BITS_D(p_lg(b), 0.0);
}
TC_CASE(negative_non_poles)
{
    volatile double a = -0.5, b = -1.5, c = -2.5;
    if (!p_lg) TC_SKIP("no lgamma");
    CHECK_NEAR(p_lg(a), 1.2655121234846454, 1e-12);
    CHECK_NEAR(p_lg(b), 0.8600470153764810, 1e-12);
    CHECK_NEAR(p_lg(c), -0.0562437164976741, 1e-12);
}
TC_CASE(nan_inf)
{
    volatile double inf = DINF, n = DNAN; double r;
    if (!p_lg) TC_SKIP("no lgamma");
    errno = 0; CHECK_BITS_D(p_lg(inf), inf); CHECK(ERR_OK());
    errno = 0; CHECK_BITS_D(p_lg(-inf), inf); CHECK(ERR_OK());
    r = p_lg(n); CHECK(r != r);
}
TC_CASE(float_variants)
{
    volatile float a = -0.5f, z = 0.0f;
    if (!p_lgf) TC_SKIP("no lgammaf");
    CHECK_NEAR(p_lgf(a), 1.2655121f, 1e-5);
    errno = 0; CHECK_EQ_U(tc_fbits(p_lgf(z)), 0x7f800000u); CHECK(ERR_OK());
}
int main(void)
{
    tc_begin();
    p_lg = (pd1)tc_crt_proc("lgamma"); p_lgf = (pf1)tc_crt_proc("lgammaf");
    TC_RUN(one_two_plus_zero); TC_RUN(negative_non_poles); TC_RUN(nan_inf); TC_RUN(float_variants);
    return tc_end();
}
