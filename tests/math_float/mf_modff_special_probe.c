/* TC
 * id:    mf_modff_special_probe
 * area:  math_float
 * funcs:
 * procs: modff
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  probe
 * ref:   C11 F.10.3.12
 * note:  Calls go to the real ucrtbase export via GetProcAddress (a bare call in a mingw exe lands in libmingwex static code, not the DLL).
 *        PROBE: C requires modf(+-inf) = +-0 with *iptr = +-inf, and a zero fraction to carry the sign of x.
 *        Older Windows CRTs are not known to follow this; each case isolated.
 */
#include "tc.h"
#include <math.h>
TC_DECL_PROC(float, modff, (float, float *));
#undef modff
#define modff tc_p_modff

static volatile float v_zero = 0.0f, v_one = 1.0f;

TC_CASE(neg_int_frac_sign) { volatile float x = -5.0f; float i = -1.0f, r = modff(x, &i); CHECK_BITS_F(r, -0.0f); }
TC_CASE(pos_inf) { volatile float inf = v_one / v_zero; float i = 0.0f, r = modff(inf, &i); CHECK_BITS_F(r, 0.0f); CHECK(i == inf); }
TC_CASE(neg_inf) { volatile float inf = -(v_one / v_zero); float i = 0.0f, r = modff(inf, &i); CHECK_BITS_F(r, -0.0f); CHECK(i == inf); }

int main(void) { tc_begin(); TC_LOAD_PROC(modff); if (!tc_p_modff) { tc_write("TC FAIL " TC_TEST_ID "::proc modff not exported\n"); return 1; } TC_RUN(neg_int_frac_sign); TC_RUN(pos_inf); TC_RUN(neg_inf); return tc_end(); }
