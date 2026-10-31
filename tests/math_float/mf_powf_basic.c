/* TC
 * id:    mf_powf_basic
 * area:  math_float
 * funcs:
 * procs: powf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 F.10.4.4; MS docs pow
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

TC_DECL_PROC(float, powf, (float, float));
#undef powf
#define powf tc_p_powf

static volatile float v_zero = 0.0f, v_one = 1.0f;
static float F_INF(void) { return v_one / v_zero; }
static float F_NAN(void) { return v_zero / v_zero; }

TC_CASE(exact) { volatile float a = 2.0f, b = 10.0f, c = -1.0f; CHECK_NEAR(powf(a, b), 1024.0, 1e-3); CHECK_NEAR(powf(a, c), 0.5, 1e-6); }
TC_CASE(y_zero) { volatile float z = 0.0f, n = F_NAN(), i = F_INF(); CHECK_BITS_F(powf(n, z), 1.0f); CHECK_BITS_F(powf(z, z), 1.0f); CHECK_BITS_F(powf(i, z), 1.0f); }
TC_CASE(x_one) { volatile float o = 1.0f, n = F_NAN(), i = F_INF(); CHECK_BITS_F(powf(o, n), 1.0f); CHECK_BITS_F(powf(o, i), 1.0f); }
TC_CASE(negative_base) { volatile float b = -2.0f, three = 3.0f, two = 2.0f; CHECK_NEAR(powf(b, three), -8.0, 1e-5); CHECK_NEAR(powf(b, two), 4.0, 1e-5); }
TC_CASE(sqrt9) { volatile float a = 9.0f, h = 0.5f; CHECK_NEAR(powf(a, h), 3.0, 1e-5); }
TC_CASE(zero_base) { volatile float p = 0.0f, n = -0.0f, one = 1.0f, three = 3.0f; CHECK_BITS_F(powf(p, one), 0.0f); CHECK_BITS_F(powf(n, three), -0.0f); }

int main(void) { tc_begin(); TC_LOAD_PROC(powf); if (!tc_p_powf) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(exact); TC_RUN(y_zero); TC_RUN(x_one); TC_RUN(negative_base); TC_RUN(sqrt9); TC_RUN(zero_base); return tc_end(); }
