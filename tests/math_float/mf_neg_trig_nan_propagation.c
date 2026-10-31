/* TC
 * id:    mf_neg_trig_nan_propagation
 * area:  math_float
 * funcs:
 * procs: sinf cosf tanf asinf acosf atanf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10 (functions return NaN for NaN argument)
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(float, sinf, (float));
#undef sinf
#define sinf tc_p_sinf
TC_DECL_PROC(float, cosf, (float));
#undef cosf
#define cosf tc_p_cosf
TC_DECL_PROC(float, tanf, (float));
#undef tanf
#define tanf tc_p_tanf
TC_DECL_PROC(float, asinf, (float));
#undef asinf
#define asinf tc_p_asinf
TC_DECL_PROC(float, acosf, (float));
#undef acosf
#define acosf tc_p_acosf
TC_DECL_PROC(float, atanf, (float));
#undef atanf
#define atanf tc_p_atanf

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;
static void init_specials(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

TC_CASE(sinf_nan) { CHECK_IS_NAN(sinf(v_nan)); }
TC_CASE(cosf_nan) { CHECK_IS_NAN(cosf(v_nan)); }
TC_CASE(tanf_nan) { CHECK_IS_NAN(tanf(v_nan)); }
TC_CASE(asinf_nan) { CHECK_IS_NAN(asinf(v_nan)); }
TC_CASE(acosf_nan) { CHECK_IS_NAN(acosf(v_nan)); }
TC_CASE(atanf_nan) { CHECK_IS_NAN(atanf(v_nan)); }
int main(void)
{
    tc_begin(); TC_LOAD_PROC(sinf); TC_LOAD_PROC(cosf); TC_LOAD_PROC(tanf); TC_LOAD_PROC(asinf); TC_LOAD_PROC(acosf); TC_LOAD_PROC(atanf); if (!(tc_p_sinf && tc_p_cosf && tc_p_tanf && tc_p_asinf && tc_p_acosf && tc_p_atanf)) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } init_specials();
    TC_RUN(sinf_nan); TC_RUN(cosf_nan); TC_RUN(tanf_nan); TC_RUN(asinf_nan); TC_RUN(acosf_nan); TC_RUN(atanf_nan);
    return tc_end();
}
