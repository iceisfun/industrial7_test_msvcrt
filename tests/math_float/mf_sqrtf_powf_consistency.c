/* TC
 * id:    mf_sqrtf_powf_consistency
 * area:  math_float
 * funcs:
 * procs: sqrtf powf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  inferred
 * ref:   C11 7.12.7.4 pow, 7.12.7.5 sqrt; exactness of pow for exactly representable results is expected quality, not mandated
 * note:  powf(3,2), powf(7,2), powf(x,1) exactness and powf(x,-1) ~ 1/x are expected of any decent implementation
 *        but are not mandated by the standard; powf(x,0.5) vs sqrtf(x) is checked within 1e-6 relative.
 */
#include "tc.h"

typedef float (__cdecl *f1_t)(float);
typedef float (__cdecl *f2_t)(float, float);
static f1_t p_sqrtf; static f2_t p_powf;
static double dabs(double d) { return d < 0 ? -d : d; }
static volatile float v_2 = 2.0f, v_3 = 3.0f, v_7 = 7.0f, v_half = 0.5f, v_one = 1.0f, v_m1 = -1.0f;

TC_CASE(small_squares_exact) { CHECK_BITS_F(p_powf(v_3, v_2), 9.0f); CHECK_BITS_F(p_powf(v_7, v_2), 49.0f); }
TC_CASE(pow_half_near_sqrt)
{
    static const float xs[] = { 2.0f, 3.0f, 10.0f, 0.25f, 12345.0f };
    int i;
    for (i = 0; i < 5; i++) { volatile float x = xs[i]; double a = p_powf(x, v_half), b = p_sqrtf(x); CHECK(dabs(a - b) <= 1e-6 * b); }
}
TC_CASE(pow_two_half) { CHECK_NEAR(p_powf(v_2, v_half), p_sqrtf(v_2), 1e-6); }
TC_CASE(pow_one_exact)
{
    static const float xs[] = { 0.1f, 2.5f, -3.0f, 1e10f, 1.17549435e-38f };
    int i;
    for (i = 0; i < 5; i++) { volatile float x = xs[i]; CHECK_BITS_F(p_powf(x, v_one), xs[i]); }
}
TC_CASE(pow_minus_one_recip)
{
    static const float xs[] = { 2.0f, 3.0f, 0.1f, 8.0f, -4.0f };
    int i;
    for (i = 0; i < 5; i++) { volatile float x = xs[i]; double a = p_powf(x, v_m1), b = 1.0 / (double)xs[i]; CHECK(dabs(a - b) <= 1e-6 * dabs(b)); }
}

int main(void)
{
    tc_begin();
    p_sqrtf = (f1_t)tc_crt_proc("sqrtf"); p_powf = (f2_t)tc_crt_proc("powf");
    if (!p_sqrtf || !p_powf) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; }
    TC_RUN(small_squares_exact); TC_RUN(pow_half_near_sqrt); TC_RUN(pow_two_half); TC_RUN(pow_one_exact); TC_RUN(pow_minus_one_recip);
    return tc_end();
}
