/* TC
 * id:    mf_neg_asinf_acosf_domain
 * area:  math_float
 * funcs:
 * procs: asinf acosf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.1.1/F.10.1.2 (|x|>1 invalid, returns NaN); MS docs asin, acos
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(float, asinf, (float));
#undef asinf
#define asinf tc_p_asinf
TC_DECL_PROC(float, acosf, (float));
#undef acosf
#define acosf tc_p_acosf

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;
static void init_specials(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

static long long iph0;
TC_CASE(asin_just_over) { volatile float x = 1.0000001f; CHECK_IS_NAN(asinf(x)); }
TC_CASE(asin_m2) { volatile float x = -2.0f; CHECK_IS_NAN(asinf(x)); }
TC_CASE(acos_2) { volatile float x = 2.0f; CHECK_IS_NAN(acosf(x)); }
TC_CASE(acos_m15) { volatile float x = -1.5f; CHECK_IS_NAN(acosf(x)); }
TC_CASE(inf) { CHECK_IS_NAN(asinf(v_inf)); CHECK_IS_NAN(asinf(-v_inf)); CHECK_IS_NAN(acosf(v_inf)); CHECK_IS_NAN(acosf(-v_inf)); }
TC_CASE(no_iph) { CHECK_EQ_I(tc_iph_count, iph0); }
int main(void)
{
    tc_begin(); TC_LOAD_PROC(asinf); TC_LOAD_PROC(acosf); if (!(tc_p_asinf && tc_p_acosf)) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } init_specials(); tc_iph_install(); iph0 = tc_iph_count;
    TC_RUN(asin_just_over); TC_RUN(asin_m2); TC_RUN(acos_2); TC_RUN(acos_m15); TC_RUN(inf); TC_RUN(no_iph);
    return tc_end();
}
