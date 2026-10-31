/* TC
 * id:    mf_neg_sinf_inf_domain
 * area:  math_float
 * funcs:
 * procs: sinf cosf tanf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.1.5-7 (sin/cos/tan of +-inf is NaN, invalid); MS docs sin, cos, tan
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

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;
static void init_specials(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

static long long iph0;
TC_CASE(sin_inf) { CHECK_IS_NAN(sinf(v_inf)); CHECK_IS_NAN(sinf(-v_inf)); }
TC_CASE(cos_inf) { CHECK_IS_NAN(cosf(-v_inf)); CHECK_IS_NAN(cosf(v_inf)); }
TC_CASE(tan_inf) { CHECK_IS_NAN(tanf(v_inf)); CHECK_IS_NAN(tanf(-v_inf)); }
TC_CASE(no_iph) { CHECK_EQ_I(tc_iph_count, iph0); }
int main(void)
{
    tc_begin(); TC_LOAD_PROC(sinf); TC_LOAD_PROC(cosf); TC_LOAD_PROC(tanf); if (!(tc_p_sinf && tc_p_cosf && tc_p_tanf)) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } init_specials(); tc_iph_install(); iph0 = tc_iph_count;
    TC_RUN(sin_inf); TC_RUN(cos_inf); TC_RUN(tan_inf); TC_RUN(no_iph);
    return tc_end();
}
