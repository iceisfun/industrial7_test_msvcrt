/* TC
 * id:    mf_neg_modff_nan
 * area:  math_float
 * funcs:
 * procs: modff
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12 / F.10 (NaN in -> NaN out); MS docs modf, modff
 * note:  only the return value is asserted; the stored integral part for NaN is unspecified by C (see mf_neg_modff_nan_iptr_probe).
 *        modff is resolved at run time (not in every mingw import lib); skipped if not exported.
 */
#include "tc.h"
typedef float (__cdecl *modff_t)(float, float *);
static modff_t p_modff;
static volatile unsigned int v_qnan = 0x7FC00000u, v_nnan = 0xFFC00000u;
static float mkf(unsigned int b) { union { unsigned int u; float f; } x; x.u = b; return x.f; }

TC_CASE(qnan)
{
    float i = 0.0f, r;
    if (!p_modff) TC_SKIP("modff not exported");
    r = p_modff(mkf(v_qnan), &i);
    CHECK(r != r);
    (void)i;
}
TC_CASE(neg_nan)
{
    float i = 0.0f, r;
    if (!p_modff) TC_SKIP("modff not exported");
    r = p_modff(mkf(v_nnan), &i);
    CHECK(r != r);
    (void)i;
}
int main(void)
{
    tc_begin();
    p_modff = (modff_t)tc_crt_proc("modff");
    TC_RUN(qnan); TC_RUN(neg_nan);
    return tc_end();
}
