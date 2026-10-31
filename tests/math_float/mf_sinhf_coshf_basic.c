/* TC
 * id:    mf_sinhf_coshf_basic
 * area:  math_float
 * funcs:
 * procs: sinhf coshf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 F.10.2.5 / F.10.2.1; MS docs sinh, cosh
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

TC_DECL_PROC(float, sinhf, (float));
#undef sinhf
#define sinhf tc_p_sinhf
TC_DECL_PROC(float, coshf, (float));
#undef coshf
#define coshf tc_p_coshf

static volatile float v_zero = 0.0f, v_one = 1.0f;
static float F_INF(void) { return v_one / v_zero; }
static float F_NAN(void) { return v_zero / v_zero; }

TC_CASE(zeros) { volatile float p = 0.0f, n = -0.0f; CHECK_BITS_F(sinhf(p), 0.0f); CHECK_BITS_F(sinhf(n), -0.0f); CHECK_BITS_F(coshf(p), 1.0f); CHECK_BITS_F(coshf(n), 1.0f); }
TC_CASE(inf) { volatile float i = F_INF(), n = -F_INF(); CHECK_BITS_F(coshf(i), i); CHECK_BITS_F(coshf(n), i); CHECK_BITS_F(sinhf(i), i); CHECK_BITS_F(sinhf(n), n); }
TC_CASE(sinh_one) { volatile float x = 1.0f; CHECK_NEAR(sinhf(x), 1.1752012, 1e-6); }
TC_CASE(cosh_one) { volatile float x = 1.0f; CHECK_NEAR(coshf(x), 1.5430806, 1e-6); }
TC_CASE(symmetry) {
    volatile float a = 0.75f, b = -0.75f;
    CHECK_NEAR(coshf(a), coshf(b), 1e-6);
    CHECK_NEAR(sinhf(a), -sinhf(b), 1e-6);
}

int main(void) { tc_begin(); TC_LOAD_PROC(sinhf); TC_LOAD_PROC(coshf); if (!tc_p_sinhf || !tc_p_coshf) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(zeros); TC_RUN(inf); TC_RUN(sinh_one); TC_RUN(cosh_one); TC_RUN(symmetry); return tc_end(); }
