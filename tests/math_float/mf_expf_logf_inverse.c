/* TC
 * id:    mf_expf_logf_inverse
 * area:  math_float
 * funcs:
 * procs: expf logf log10f powf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  inferred
 * ref:   C11 7.12.6.1 exp, 7.12.6.7 log, 7.12.6.8 log10, 7.12.7.4 pow (mathematical identities, tolerance-based)
 */
#include "tc.h"

typedef float (__cdecl *f1_t)(float);
typedef float (__cdecl *f2_t)(float, float);
static f1_t p_expf, p_logf, p_log10f; static f2_t p_powf;
static double dabs(double d) { return d < 0 ? -d : d; }
static volatile float v_ten = 10.0f;

TC_CASE(log_of_exp)
{
    int i;
    for (i = -5; i <= 5; i++) { volatile float x = (float)i; CHECK_NEAR(p_logf(p_expf(x)), (double)x, 1e-5); }
}
TC_CASE(pow10_of_log10)
{
    static const float xs[] = { 0.5f, 2.0f, 3.0f, 7.0f, 100.0f, 1234.0f };
    int i;
    for (i = 0; i < 6; i++) { volatile float x = xs[i]; double r = p_powf(v_ten, p_log10f(x)); CHECK(dabs(r - x) <= 1e-5 * x); }
}
TC_CASE(exp_sum)
{
    static const float a[] = { -2.0f, 0.5f, 1.0f, 3.0f }, b[] = { 1.0f, 0.25f, -0.5f, 2.0f };
    int i;
    for (i = 0; i < 4; i++) {
        volatile float x = a[i], y = b[i], s = a[i] + b[i];
        double l = p_expf(s), r = (double)p_expf(x) * (double)p_expf(y);
        CHECK(dabs(l - r) <= 1e-5 * r);
    }
}
TC_CASE(log10_of_pow10)
{
    int i;
    for (i = -3; i <= 3; i++) { volatile float x = (float)i; CHECK_NEAR(p_log10f(p_powf(v_ten, x)), (double)x, 1e-5); }
}

int main(void)
{
    tc_begin();
    p_expf = (f1_t)tc_crt_proc("expf"); p_logf = (f1_t)tc_crt_proc("logf"); p_log10f = (f1_t)tc_crt_proc("log10f"); p_powf = (f2_t)tc_crt_proc("powf");
    if (!p_expf || !p_logf || !p_log10f || !p_powf) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; }
    TC_RUN(log_of_exp); TC_RUN(pow10_of_log10); TC_RUN(exp_sum); TC_RUN(log10_of_pow10);
    return tc_end();
}
