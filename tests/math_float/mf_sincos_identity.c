/* TC
 * id:    mf_sincos_identity
 * area:  math_float
 * funcs:
 * procs: sinf cosf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.12.4.5/7.12.4.6; MS docs sin, cos
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

TC_CASE(pythagorean)
{
    int i;
    for (i = 1; i <= 60; i++) {
        volatile float x = (float)i * 0.1f;
        double s = sinf(x), c = cosf(x), d = s * s + c * c - 1.0;
        if (d < 0) d = -d;
        if (d > 1e-6) tc_fail_expr(__FILE__, __LINE__, "sin^2+cos^2 != 1");
    }
}
TC_CASE(periodic)
{
    int i;
    for (i = 1; i <= 30; i++) {
        volatile float x = (float)i * 0.2f, y = x + 6.2831855f;
        CHECK_NEAR(sinf(y), sinf(x), 1e-5);
    }
}
int main(void) { tc_begin(); TC_LOAD_PROC(sinf); TC_LOAD_PROC(cosf); if (!(tc_p_sinf && tc_p_cosf)) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(pythagorean); TC_RUN(periodic); return tc_end(); }
