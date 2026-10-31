/* TC
 * id:    mf_copysignf_c99
 * area:  math_float
 * funcs:
 * procs: copysignf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.11.1 copysign
 * note:  copysignf resolved at run time (mingw headers supply it inline); skipped if not exported.
 */
#include "tc.h"
typedef float (__cdecl *cs_t)(float, float);
static cs_t copysignf_p;
#define copysignf copysignf_p
static volatile float p2 = 2, m2 = -2, p0 = 0.0f, m0 = -0.0f, m1 = -1;

TC_CASE(neg_zero_sign) { if (!copysignf_p) TC_SKIP("not exported"); CHECK_BITS_F(copysignf(p2, m0), -2.0f); }
TC_CASE(pos_zero_sign) { if (!copysignf_p) TC_SKIP("not exported"); CHECK_BITS_F(copysignf(m2, p0), 2.0f); }
TC_CASE(zero_neg) { if (!copysignf_p) TC_SKIP("not exported"); CHECK_BITS_F(copysignf(p0, m1), -0.0f); }
int main(void) { tc_begin(); copysignf_p = (cs_t)tc_crt_proc("copysignf"); TC_RUN(neg_zero_sign); TC_RUN(pos_zero_sign); TC_RUN(zero_neg); return tc_end(); }
