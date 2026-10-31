/* TC
 * id:    mf_neg_asinf_acosf_boundary_ok
 * area:  math_float
 * funcs:
 * procs: asinf acosf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.4.1/2, F.10.1.1/2; MS docs asin, acos
 * note:  errno after the out-of-domain 1+ulp calls is deliberately not asserted.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>
#include <limits.h>

static volatile float v_zero = 0.0f, v_one = 1.0f;
static float F_INF(void) { return v_one / v_zero; }
static float F_NAN(void) { return v_zero / v_zero; }
#define SENT 0x5a5
#define NEED(p) do { if (!(p)) TC_SKIP("function not exported"); } while (0)

TC_DECL_PROC(float, asinf, (float));
TC_DECL_PROC(float, acosf, (float));
static float from_bits(unsigned int u) { union { unsigned int u; float f; } x; x.u = u; return x.f; }
static volatile unsigned int v_one_ulp = 0x3f800001u, v_m_one_ulp = 0xbf800001u;

TC_CASE(asinf_one_exact) { volatile float x = 1.0f; float r; NEED(tc_p_asinf); errno = SENT; r = tc_p_asinf(x); CHECK_NEAR(r, 1.5707964, 1e-6); CHECK_EQ_I(errno, SENT); }
TC_CASE(asinf_m1_exact) { volatile float x = -1.0f; float r; NEED(tc_p_asinf); errno = SENT; r = tc_p_asinf(x); CHECK_NEAR(r, -1.5707964, 1e-6); CHECK_EQ_I(errno, SENT); }
TC_CASE(acosf_one_zero_errno_clean) { volatile float x = 1.0f; float r; NEED(tc_p_acosf); errno = SENT; r = tc_p_acosf(x); CHECK_BITS_F(r, 0.0f); CHECK_EQ_I(errno, SENT); }
TC_CASE(acosf_m1_pi) { volatile float x = -1.0f; float r; NEED(tc_p_acosf); errno = SENT; r = tc_p_acosf(x); CHECK_NEAR(r, 3.1415927, 1e-6); CHECK_EQ_I(errno, SENT); }
TC_CASE(one_plus_ulp_nan)
{
    volatile float x = from_bits(v_one_ulp);
    NEED(tc_p_asinf); NEED(tc_p_acosf);
    CHECK(x > 1.0f);
    CHECK_IS_NAN(tc_p_asinf(x)); CHECK_IS_NAN(tc_p_acosf(x));
}
TC_CASE(m1_minus_ulp_nan)
{
    volatile float x = from_bits(v_m_one_ulp);
    NEED(tc_p_asinf); NEED(tc_p_acosf);
    CHECK(x < -1.0f);
    CHECK_IS_NAN(tc_p_asinf(x)); CHECK_IS_NAN(tc_p_acosf(x));
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(asinf); TC_LOAD_PROC(acosf);
    TC_RUN(asinf_one_exact); TC_RUN(asinf_m1_exact); TC_RUN(acosf_one_zero_errno_clean); TC_RUN(acosf_m1_pi); TC_RUN(one_plus_ulp_nan); TC_RUN(m1_minus_ulp_nan);
    errno = 0; return tc_end();
}
