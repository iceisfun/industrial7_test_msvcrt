/* TC
 * id:    mf_neg_modff_nan_iptr_probe
 * area:  math_float
 * funcs:
 * procs: modff
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.6.12 (stored value for NaN unspecified)
 * note:  PROBE: modff(qNaN, &i) stores a NaN in i on Windows (most likely; the C standard does not say).
 */
#include "tc.h"
typedef float (__cdecl *modff_t)(float, float *);
static volatile unsigned int v_qnan = 0x7FC00000u;
static float mkf(unsigned int b) { union { unsigned int u; float f; } x; x.u = b; return x.f; }
TC_CASE(iptr_nan)
{
    float i = 0.0f;
    modff_t p = (modff_t)tc_crt_proc("modff");
    if (!p) TC_SKIP("modff not exported");
    p(mkf(v_qnan), &i);
    CHECK(i != i);
}
int main(void) { tc_begin(); TC_RUN(iptr_nan); return tc_end(); }
