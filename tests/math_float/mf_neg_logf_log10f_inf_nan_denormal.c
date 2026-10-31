/* TC
 * id:    mf_neg_logf_log10f_inf_nan_denormal
 * area:  math_float
 * funcs:
 * procs: logf log10f
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.6.11/7.12.6.8, F.10.3.11; MS docs log, log10
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

TC_DECL_PROC(float, logf, (float));
TC_DECL_PROC(float, log10f, (float));
static float from_bits(unsigned int u) { union { unsigned int u; float f; } x; x.u = u; return x.f; }
static volatile unsigned int v_mind = 1u, v_negd = 0x80000001u;

TC_CASE(logf_pinf) { volatile float x = F_INF(); NEED(tc_p_logf); CHECK_BITS_F(tc_p_logf(x), F_INF()); }
TC_CASE(log10f_pinf) { volatile float x = F_INF(); NEED(tc_p_log10f); CHECK_BITS_F(tc_p_log10f(x), F_INF()); }
TC_CASE(logf_min_denormal_finite)
{
    volatile float x = from_bits(v_mind); float r; NEED(tc_p_logf);
    r = tc_p_logf(x);
    CHECK_NEAR(r, -103.27893, 0.01);   /* ln(2^-149) = -149*ln2 */
}
TC_CASE(log10f_neg_denormal_nan)
{
    volatile float x = from_bits(v_negd); NEED(tc_p_log10f); NEED(tc_p_logf);
    CHECK_IS_NAN(tc_p_log10f(x)); CHECK_IS_NAN(tc_p_logf(x));
}
TC_CASE(logf_nan_errno_unchanged) { volatile float x = F_NAN(); float r; NEED(tc_p_logf); errno = SENT; r = tc_p_logf(x); CHECK_IS_NAN(r); CHECK_EQ_I(errno, SENT); }
TC_CASE(logf_pinf_errno_unchanged) { volatile float x = F_INF(); NEED(tc_p_logf); errno = SENT; tc_p_logf(x); CHECK_EQ_I(errno, SENT); }
int main(void)
{
    tc_begin(); TC_LOAD_PROC(logf); TC_LOAD_PROC(log10f);
    TC_RUN(logf_pinf); TC_RUN(log10f_pinf); TC_RUN(logf_min_denormal_finite); TC_RUN(log10f_neg_denormal_nan); TC_RUN(logf_nan_errno_unchanged); TC_RUN(logf_pinf_errno_unchanged);
    errno = 0; return tc_end();
}
