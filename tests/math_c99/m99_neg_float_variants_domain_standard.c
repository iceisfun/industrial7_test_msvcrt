/* TC
 * id:    m99_neg_float_variants_domain_standard
 * area:  math_c99
 * funcs: log2f log1pf tgammaf expm1f erfcf cbrtf fmaf lroundf
 * procs: nexttowardf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10 (log2, log1p, tgamma, expm1, erfc, cbrt, fma, lround, nexttoward)
 * note:  f-suffixed functions must return float-typed NaN/Inf, not a double-promoted value. lroundf of NaN/out-of-range is unspecified: only errno in {0,EDOM,ERANGE} is asserted.
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

typedef float (__cdecl *ntf_t)(float, double);
static ntf_t p_ntf;
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
#define FINF (f_one / f_zero)
#define FNAN (f_zero / f_zero)
#define ERR_OK() (errno == 0 || errno == ERANGE || errno == EDOM)

TC_CASE(log_family)
{
    volatile float m1 = -1.0f, z = 0.0f, m2 = -2.0f; float r;
    r = log2f(m1); CHECK(r != r);
    errno = 0; CHECK_EQ_U(tc_fbits(log2f(z)), 0xff800000u); CHECK(ERR_OK());
    errno = 0; CHECK_EQ_U(tc_fbits(log1pf(m1)), 0xff800000u); CHECK(ERR_OK());
    r = log1pf(m2); CHECK(r != r);
}
TC_CASE(tgammaf_edges)
{
    volatile float m1 = -1.0f, z = 0.0f, big = 172.0f; float r;
    r = tgammaf(m1); CHECK(r != r);
    errno = 0; CHECK_EQ_U(tc_fbits(tgammaf(z)), 0x7f800000u); CHECK(ERR_OK());
    errno = 0; CHECK_EQ_U(tc_fbits(tgammaf(big)), 0x7f800000u); CHECK(ERR_OK());
}
TC_CASE(expm1f_erfcf_cbrtf)
{
    volatile float a = 89.0f, ninf = -FINF;
    errno = 0; CHECK_EQ_U(tc_fbits(expm1f(a)), 0x7f800000u); CHECK(ERR_OK());
    CHECK_BITS_F(expm1f(ninf), -1.0f);
    CHECK_BITS_F(cbrtf(ninf), ninf);
    CHECK_BITS_F(erfcf(ninf), 2.0f);
}
TC_CASE(fmaf_invalid)
{
    volatile float inf = FINF, z = 0.0f, one = 1.0f, two = 2.0f, three = 3.0f; float r;
    r = fmaf(inf, z, one); CHECK(r != r);
    CHECK_BITS_F(fmaf(two, three, one), 7.0f);
}
TC_CASE(lroundf_unspecified)
{
    volatile float n = FNAN, big = 1e20f; volatile long r;
    errno = 0; r = lroundf(n); (void)r; CHECK(ERR_OK());
    errno = 0; r = lroundf(big); (void)r; CHECK(ERR_OK());
}
TC_CASE(nexttowardf_overflow)
{
    volatile float mx = FLT_MAX; volatile double inf = v_one / v_zero;
    if (!p_ntf) TC_SKIP("missing");
    errno = 0; CHECK_EQ_U(tc_fbits(p_ntf(mx, inf)), 0x7f800000u); CHECK(ERR_OK());
}
int main(void)
{
    tc_begin();
    p_ntf = (ntf_t)tc_crt_proc("nexttowardf");
    TC_RUN(log_family); TC_RUN(tgammaf_edges); TC_RUN(expm1f_erfcf_cbrtf); TC_RUN(fmaf_invalid); TC_RUN(lroundf_unspecified); TC_RUN(nexttowardf_overflow);
    return tc_end();
}
