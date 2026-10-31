/* TC
 * id:    mf_neg_fatal_none_math_no_abort
 * area:  math_float
 * funcs:
 * procs: sqrtf logf powf fmodf asinf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sqrt, log, pow, fmod, asin: domain errors return NaN (and set errno), no exception with the default FP control word
 * note:  Child process (handler installed, counted) calls bad-domain float math and must exit normally with 77: no abort, no 0xC0000409, no FP exception (0xC000008E/0xC0000090).
 */
#include "tc.h"
TC_DECL_PROC(float, sqrtf, (float)); TC_DECL_PROC(float, logf, (float)); TC_DECL_PROC(float, powf, (float, float));
TC_DECL_PROC(float, fmodf, (float, float)); TC_DECL_PROC(float, asinf, (float));
static volatile float v_m1 = -1.0f, v_one = 1.0f, v_zero = 0.0f, v_2 = 2.0f, v_half = 0.5f;

static void load(void) { TC_LOAD_PROC(sqrtf); TC_LOAD_PROC(logf); TC_LOAD_PROC(powf); TC_LOAD_PROC(fmodf); TC_LOAD_PROC(asinf); }
static int child_main(const char *n)
{
    volatile float r = 0; (void)n;
    tc_iph_install();
    load();
    if (tc_p_sqrtf) r += tc_p_sqrtf(v_m1);
    if (tc_p_logf) r += tc_p_logf(v_m1);
    if (tc_p_fmodf) r += tc_p_fmodf(v_one, v_zero);
    if (tc_p_asinf) r += tc_p_asinf(v_2);
    if (tc_p_powf) r += tc_p_powf(v_m1, v_half);
    if (tc_iph_count) return 79;   /* math domain errors must not reach the invalid-parameter handler */
    return 77;
}
TC_CASE(child_survives)
{
    DWORD code = 0;
    int ran = tc_spawn_self("badmath", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 77);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(child_survives);
    return tc_end();
}
