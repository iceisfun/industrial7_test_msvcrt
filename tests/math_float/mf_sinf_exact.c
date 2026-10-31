/* TC
 * id:    mf_sinf_exact
 * area:  math_float
 * funcs:
 * procs: sinf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 F.10.1.6; MS docs sin, sinf
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(float, sinf, (float));
#undef sinf
#define sinf tc_p_sinf

TC_CASE(zeros) { volatile float p = 0.0f, n = -0.0f; CHECK_BITS_F(sinf(p), 0.0f); CHECK_BITS_F(sinf(n), -0.0f); }
TC_CASE(pi_over_6) { volatile float x = 0.5235988f; CHECK_NEAR(sinf(x), 0.5, 1e-6); }
TC_CASE(one) { volatile float x = 1.0f; CHECK_NEAR(sinf(x), 0.84147098, 1e-6); }
TC_CASE(odd)
{
    static const float xs[5] = { 0.1f, 0.5f, 1.0f, 2.0f, 3.0f };
    int i;
    for (i = 0; i < 5; i++) { volatile float x = xs[i], nx = -xs[i]; CHECK_NEAR(sinf(nx), -sinf(x), 1e-6); }
}
int main(void) { tc_begin(); TC_LOAD_PROC(sinf); if (!(tc_p_sinf)) { tc_write("TC FAIL "TC_TEST_ID"::proc missing\n"); return 1; } TC_RUN(zeros); TC_RUN(pi_over_6); TC_RUN(one); TC_RUN(odd); return tc_end(); }
