/* TC
 * id:    mf_neg_logf_domain_pole
 * area:  math_float
 * funcs:
 * procs: logf log10f
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.3.11 / F.10.3.12
 * note:  values only; errno in mf_neg_logf_errno. iph must not run for domain errors.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

TC_DECL_PROC(float, logf, (float));
#undef logf
#define logf tc_p_logf
TC_DECL_PROC(float, log10f, (float));
#undef log10f
#define log10f tc_p_log10f

static volatile float v_zero = 0.0f, v_one = 1.0f;
static float F_INF(void) { return v_one / v_zero; }
static float F_NAN(void) { return v_zero / v_zero; }

TC_CASE(logf_zero) { volatile float p = 0.0f, n = -0.0f; CHECK_BITS_F(logf(p), -F_INF()); CHECK_BITS_F(logf(n), -F_INF()); }
TC_CASE(logf_negative) { volatile float x = -1.0f, n = -F_INF(); CHECK_IS_NAN(logf(x)); CHECK_IS_NAN(logf(n)); }
TC_CASE(log10f_zero) { volatile float x = 0.0f; CHECK_BITS_F(log10f(x), -F_INF()); }
TC_CASE(log10f_negative) { volatile float x = -5.0f; CHECK_IS_NAN(log10f(x)); }
TC_CASE(no_iph) {
    volatile float a = -1.0f, b = 0.0f;
    tc_iph_install(); tc_iph_count = 0;
    (void)logf(a); (void)logf(b); (void)log10f(a); (void)log10f(b);
    CHECK_EQ_I(tc_iph_count, 0);
}

int main(void) { tc_begin(); TC_LOAD_PROC(logf); TC_LOAD_PROC(log10f); if (!tc_p_logf || !tc_p_log10f) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(logf_zero); TC_RUN(logf_negative); TC_RUN(log10f_zero); TC_RUN(log10f_negative); TC_RUN(no_iph); return tc_end(); }
