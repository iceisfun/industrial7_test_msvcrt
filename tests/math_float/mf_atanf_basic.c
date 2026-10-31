/* TC
 * id:    mf_atanf_basic
 * area:  math_float
 * funcs:
 * procs: atanf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 F.10.1.3; MS docs atan, atanf
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(float, atanf, (float));
#undef atanf
#define atanf tc_p_atanf

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;
static void init_specials(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

TC_CASE(zeros) { volatile float p = 0.0f, n = -0.0f; CHECK_BITS_F(atanf(p), 0.0f); CHECK_BITS_F(atanf(n), -0.0f); }
TC_CASE(one) { volatile float x = 1.0f; CHECK_NEAR(atanf(x), 0.7853981633974483, 1e-6); }
TC_CASE(inf) { CHECK_NEAR(atanf(v_inf), 1.5707963267948966, 1e-6); CHECK_NEAR(atanf(-v_inf), -1.5707963267948966, 1e-6); }
TC_CASE(odd)
{
    static const float xs[4] = { 0.1f, 0.5f, 2.0f, 100.0f };
    int i;
    for (i = 0; i < 4; i++) { volatile float x = xs[i], nx = -xs[i]; CHECK_NEAR(atanf(nx), -atanf(x), 1e-6); }
}
TC_CASE(nan) { CHECK_IS_NAN(atanf(v_nan)); }
int main(void) { tc_begin(); TC_LOAD_PROC(atanf); if (!(tc_p_atanf)) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } init_specials(); TC_RUN(zeros); TC_RUN(one); TC_RUN(inf); TC_RUN(odd); TC_RUN(nan); return tc_end(); }
