/* TC
 * id:    mf_neg_success_errno_untouched
 * area:  math_float
 * funcs:
 * procs: sinf cosf tanf atanf atan2f floorf ceilf fabsf sqrtf logf powf fmodf _hypotf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.5 (errno is set only on errors); MS docs errno, _doserrno
 * note:  errno is pre-set to a sentinel before each group of valid calls and must still hold it afterwards.
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

TC_DECL_PROC(float, sinf, (float)); TC_DECL_PROC(float, cosf, (float)); TC_DECL_PROC(float, tanf, (float));
TC_DECL_PROC(float, atanf, (float)); TC_DECL_PROC(float, atan2f, (float, float));
TC_DECL_PROC(float, floorf, (float)); TC_DECL_PROC(float, ceilf, (float)); TC_DECL_PROC(float, fabsf, (float));
TC_DECL_PROC(float, sqrtf, (float)); TC_DECL_PROC(float, logf, (float)); TC_DECL_PROC(float, powf, (float, float));
TC_DECL_PROC(float, fmodf, (float, float)); TC_DECL_PROC(float, _hypotf, (float, float));
static volatile float v_a = 0.5f, v_b = 2.0f, v_c = 2.5f, v_d = -2.5f, v_e = 5.5f, v_3 = 3.0f, v_4 = 4.0f;

TC_CASE(sinf_cosf_tanf_ok)
{
    NEED(tc_p_sinf); NEED(tc_p_cosf); NEED(tc_p_tanf);
    errno = SENT;
    CHECK_NEAR(tc_p_sinf(v_a), 0.4794255, 1e-6); CHECK_NEAR(tc_p_cosf(v_a), 0.8775826, 1e-6); CHECK_NEAR(tc_p_tanf(v_a), 0.5463025, 1e-6);
    CHECK_EQ_I(errno, SENT);
}
TC_CASE(atanf_atan2f_ok)
{
    volatile float one = 1.0f;
    NEED(tc_p_atanf); NEED(tc_p_atan2f);
    errno = SENT;
    CHECK_NEAR(tc_p_atanf(one), 0.7853982, 1e-6); CHECK_NEAR(tc_p_atan2f(one, one), 0.7853982, 1e-6);
    CHECK_EQ_I(errno, SENT);
}
TC_CASE(sqrtf_logf_ok)
{
    NEED(tc_p_sqrtf); NEED(tc_p_logf);
    errno = SENT;
    CHECK_BITS_F(tc_p_sqrtf(v_4), 2.0f); CHECK_NEAR(tc_p_logf(v_b), 0.6931472, 1e-6);
    CHECK_EQ_I(errno, SENT);
}
TC_CASE(powf_fmodf_ok)
{
    NEED(tc_p_powf); NEED(tc_p_fmodf);
    errno = SENT;
    CHECK_NEAR(tc_p_powf(v_b, v_3), 8.0, 1e-4); CHECK_BITS_F(tc_p_fmodf(v_e, v_b), 1.5f);
    CHECK_EQ_I(errno, SENT);
}
TC_CASE(hypotf_ok)
{
    NEED(tc_p__hypotf);
    errno = SENT;
    CHECK_NEAR(tc_p__hypotf(v_3, v_4), 5.0, 1e-5);
    CHECK_EQ_I(errno, SENT);
}
TC_CASE(floorf_ceilf_fabsf_ok)
{
    NEED(tc_p_floorf); NEED(tc_p_ceilf); NEED(tc_p_fabsf);
    errno = SENT;
    CHECK_BITS_F(tc_p_floorf(v_c), 2.0f); CHECK_BITS_F(tc_p_ceilf(v_c), 3.0f); CHECK_BITS_F(tc_p_fabsf(v_d), 2.5f);
    CHECK_EQ_I(errno, SENT);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(sinf); TC_LOAD_PROC(cosf); TC_LOAD_PROC(tanf); TC_LOAD_PROC(atanf); TC_LOAD_PROC(atan2f); TC_LOAD_PROC(floorf);
    TC_LOAD_PROC(ceilf); TC_LOAD_PROC(fabsf); TC_LOAD_PROC(sqrtf); TC_LOAD_PROC(logf); TC_LOAD_PROC(powf); TC_LOAD_PROC(fmodf); TC_LOAD_PROC(_hypotf);
    TC_RUN(sinf_cosf_tanf_ok); TC_RUN(atanf_atan2f_ok); TC_RUN(sqrtf_logf_ok); TC_RUN(powf_fmodf_ok); TC_RUN(hypotf_ok); TC_RUN(floorf_ceilf_fabsf_ok);
    errno = 0; return tc_end();
}
