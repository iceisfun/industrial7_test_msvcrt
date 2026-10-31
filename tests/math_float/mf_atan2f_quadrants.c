/* TC
 * id:    mf_atan2f_quadrants
 * area:  math_float
 * funcs:
 * procs: atan2f
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 F.10.1.4; MS docs atan2, atan2f
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(float, atan2f, (float, float));
#undef atan2f
#define atan2f tc_p_atan2f

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;
static void init_specials(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

#define PI  3.14159265358979323846
static float A(float y, float x) { volatile float vy = y, vx = x; return atan2f(vy, vx); }
TC_CASE(axes) { CHECK_NEAR(A(1.0f, 0.0f), PI / 2, 1e-6); CHECK_NEAR(A(-1.0f, 0.0f), -PI / 2, 1e-6); }
TC_CASE(diag) { CHECK_NEAR(A(1.0f, 1.0f), PI / 4, 1e-6); CHECK_NEAR(A(-1.0f, -1.0f), -3 * PI / 4, 1e-6); }
TC_CASE(neg_x_axis) { CHECK_NEAR(A(0.0f, -1.0f), PI, 1e-6); }
int main(void)
{
    tc_begin(); TC_LOAD_PROC(atan2f); if (!(tc_p_atan2f)) { tc_write("TC FAIL "TC_TEST_ID"::proc missing\n"); return 1; } init_specials();
    TC_RUN(axes); TC_RUN(diag); TC_RUN(neg_x_axis);
    return tc_end();
}
