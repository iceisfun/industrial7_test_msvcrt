/* TC
 * id:    mf_neg_ldexpf_overflow_proc
 * area:  math_float
 * funcs:
 * procs: ldexpf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.6.6 / F.10.3.6 ldexp: overflow -> +-HUGE_VAL, underflow -> 0, inf and NaN pass through
 * note:  skipped if ldexpf is not exported.
 */
#include "tc.h"
typedef float (__cdecl *ldexpf_t)(float, int);
static ldexpf_t p_ldexpf;
static volatile float v_1 = 1.0f, v_m1 = -1.0f, v_inf, v_nan;
static volatile int v_200 = 200, v_m200 = -200, v_3 = 3, v_m5 = -5;
static void init(void) { union { unsigned int u; float f; } x; x.u = 0x7F800000u; v_inf = x.f; x.u = 0x7FC00000u; v_nan = x.f; }

TC_CASE(overflow_pos) { if (!p_ldexpf) TC_SKIP("ldexpf not exported"); CHECK_BITS_F(p_ldexpf(v_1, v_200), v_inf); }
TC_CASE(overflow_neg) { if (!p_ldexpf) TC_SKIP("ldexpf not exported"); CHECK_BITS_F(p_ldexpf(v_m1, v_200), -v_inf); }
TC_CASE(underflow) { if (!p_ldexpf) TC_SKIP("ldexpf not exported"); CHECK_BITS_F(p_ldexpf(v_1, v_m200), 0.0f); }
TC_CASE(inf_nan)
{
    float r;
    if (!p_ldexpf) TC_SKIP("ldexpf not exported");
    CHECK_BITS_F(p_ldexpf(v_inf, v_m5), v_inf);
    r = p_ldexpf(v_nan, v_3); CHECK(r != r);
}
int main(void)
{
    tc_begin(); init();
    p_ldexpf = (ldexpf_t)tc_crt_proc("ldexpf");
    TC_RUN(overflow_pos); TC_RUN(overflow_neg); TC_RUN(underflow); TC_RUN(inf_nan);
    return tc_end();
}
