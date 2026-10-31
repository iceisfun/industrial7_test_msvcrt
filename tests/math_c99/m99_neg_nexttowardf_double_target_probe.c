/* TC
 * id:    m99_neg_nexttowardf_double_target_probe
 * area:  math_c99
 * funcs: 
 * procs: nexttowardf nextafterf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.11.4 (y is compared in long double, not converted to float)
 * note:  PROBE: nexttowardf(1.0f, 1.0000000001) must step to the next float above 1.0f (0x3f800001) because the target is compared at full (here 64-bit long double) precision. An implementation that narrows y to float first would return 1.0f. long double is 64-bit in the MS ABI, so the proc is typed as taking double.
 */
#include "tc.h"
#include <math.h>

typedef float (__cdecl *ntf_t)(float, double);
typedef float (__cdecl *naf_t)(float, float);
static ntf_t p_ntf; static naf_t p_naf;

TC_CASE(target_above_float_resolution)
{
    volatile float one = 1.0f, two = 2.0f;
    volatile double t = 1.0000000001;
    if (!p_ntf || !p_naf) TC_SKIP("missing");
    CHECK_EQ_U(tc_fbits(p_ntf(one, t)), 0x3f800001u);
    CHECK_BITS_F(p_ntf(one, t), p_naf(one, two));
}
int main(void)
{
    tc_begin();
    p_ntf = (ntf_t)tc_crt_proc("nexttowardf"); p_naf = (naf_t)tc_crt_proc("nextafterf");
    TC_RUN(target_above_float_resolution);
    return tc_end();
}
