/* TC
 * id:    mf_cosf_exact
 * area:  math_float
 * funcs:
 * procs: cosf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 F.10.1.5; MS docs cos, cosf
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(float, cosf, (float));
#undef cosf
#define cosf tc_p_cosf

TC_CASE(zeros) { volatile float p = 0.0f, n = -0.0f; CHECK_BITS_F(cosf(p), 1.0f); CHECK_BITS_F(cosf(n), 1.0f); }
TC_CASE(even)
{
    static const float xs[5] = { 0.1f, 0.5f, 1.0f, 2.0f, 3.0f };
    int i;
    for (i = 0; i < 5; i++) { volatile float x = xs[i], nx = -xs[i]; CHECK_NEAR(cosf(nx), cosf(x), 1e-6); }
}
TC_CASE(one) { volatile float x = 1.0f; CHECK_NEAR(cosf(x), 0.54030231, 1e-6); }
TC_CASE(bounded)
{
    int i;
    for (i = -100; i <= 100; i++) { volatile float x = (float)i * 0.37f; float c = cosf(x); CHECK(c <= 1.0f && c >= -1.0f); }
}
int main(void) { tc_begin(); TC_LOAD_PROC(cosf); if (!(tc_p_cosf)) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(zeros); TC_RUN(even); TC_RUN(one); TC_RUN(bounded); return tc_end(); }
