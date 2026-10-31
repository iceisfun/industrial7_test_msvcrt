/* TC
 * id:    mf_sqrtf_basic
 * area:  math_float
 * funcs:
 * procs: sqrtf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.4.5; IEEE 754 sqrt is correctly rounded
 */
#include "tc.h"
#include <math.h>

TC_DECL_PROC(float, sqrtf, (float));
#define sqrtf tc_p_sqrtf

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf;

TC_CASE(zeros) { volatile float p = 0.0f, n = -0.0f; CHECK_BITS_F(sqrtf(p), 0.0f); CHECK_BITS_F(sqrtf(n), -0.0f); }
TC_CASE(perfect)
{
    volatile float a = 4.0f, b = 9.0f, c = 2.25f, d = 1e4f;
    CHECK_BITS_F(sqrtf(a), 2.0f); CHECK_BITS_F(sqrtf(b), 3.0f); CHECK_BITS_F(sqrtf(c), 1.5f); CHECK_BITS_F(sqrtf(d), 100.0f);
}
TC_CASE(inf) { CHECK(sqrtf(v_inf) == v_inf); }
TC_CASE(two) { volatile float t = 2.0f; CHECK_EQ_U(tc_fbits(sqrtf(t)), 0x3FB504F3u); }
TC_CASE(roundtrip)
{
    static volatile float xs[10] = { 1.0f, 2.0f, 3.0f, 5.0f, 7.0f, 10.0f, 100.0f, 255.0f, 1000.0f, 4096.0f };
    int i;
    for (i = 0; i < 10; i++) { volatile float x = xs[i]; volatile float sq = x * x; CHECK_BITS_F(sqrtf(sq), x); }
}

int main(void)
{
    tc_begin(); TC_LOAD_PROC(sqrtf); if (!tc_p_sqrtf) { tc_write("TC FAIL " TC_TEST_ID "::sqrtf not exported\n"); return 1; } v_inf = v_one / v_zero;
    TC_RUN(zeros); TC_RUN(perfect); TC_RUN(inf); TC_RUN(two); TC_RUN(roundtrip);
    return tc_end();
}
