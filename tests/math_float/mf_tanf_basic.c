/* TC
 * id:    mf_tanf_basic
 * area:  math_float
 * funcs:
 * procs: tanf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 F.10.1.7; MS docs tan, tanf
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(float, tanf, (float));
#undef tanf
#define tanf tc_p_tanf

TC_CASE(zeros) { volatile float p = 0.0f, n = -0.0f; CHECK_BITS_F(tanf(p), 0.0f); CHECK_BITS_F(tanf(n), -0.0f); }
TC_CASE(pi_over_4) { volatile float x = 0.7853982f; CHECK_NEAR(tanf(x), 1.0, 1e-6); }
TC_CASE(odd)
{
    static const float xs[4] = { 0.1f, 0.5f, 1.0f, 1.3f };
    int i;
    for (i = 0; i < 4; i++) { volatile float x = xs[i], nx = -xs[i]; CHECK_NEAR(tanf(nx), -tanf(x), 1e-5); }
}
TC_CASE(one) { volatile float x = 1.0f; CHECK_NEAR(tanf(x), 1.5574077, 1e-5); }
int main(void) { tc_begin(); TC_LOAD_PROC(tanf); if (!(tc_p_tanf)) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(zeros); TC_RUN(pi_over_4); TC_RUN(odd); TC_RUN(one); return tc_end(); }
