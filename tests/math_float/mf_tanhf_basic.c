/* TC
 * id:    mf_tanhf_basic
 * area:  math_float
 * funcs:
 * procs: tanhf
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 F.10.2.6; MS docs tanh
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

TC_DECL_PROC(float, tanhf, (float));
#undef tanhf
#define tanhf tc_p_tanhf

static volatile float v_zero = 0.0f, v_one = 1.0f;
static float F_INF(void) { return v_one / v_zero; }
static float F_NAN(void) { return v_zero / v_zero; }

TC_CASE(zeros) { volatile float p = 0.0f, n = -0.0f; CHECK_BITS_F(tanhf(p), 0.0f); CHECK_BITS_F(tanhf(n), -0.0f); }
TC_CASE(inf) { volatile float i = F_INF(), n = -F_INF(); CHECK_BITS_F(tanhf(i), 1.0f); CHECK_BITS_F(tanhf(n), -1.0f); }
TC_CASE(saturate) { volatile float a = 20.0f, b = -20.0f; CHECK_BITS_F(tanhf(a), 1.0f); CHECK_BITS_F(tanhf(b), -1.0f); }
TC_CASE(half) { volatile float x = 0.5f; CHECK_NEAR(tanhf(x), 0.46211716, 1e-6); }
TC_CASE(nan) { volatile float x = F_NAN(); CHECK_IS_NAN(tanhf(x)); }

int main(void) { tc_begin(); TC_LOAD_PROC(tanhf); if (!tc_p_tanhf) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(zeros); TC_RUN(inf); TC_RUN(saturate); TC_RUN(half); TC_RUN(nan); return tc_end(); }
