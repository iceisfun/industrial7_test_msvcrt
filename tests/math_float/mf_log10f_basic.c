/* TC
 * id:    mf_log10f_basic
 * area:  math_float
 * funcs:
 * procs: log10f
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 F.10.3.12; MS docs log10
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

TC_DECL_PROC(float, log10f, (float));
#undef log10f
#define log10f tc_p_log10f

static volatile float v_zero = 0.0f, v_one = 1.0f;
static float F_INF(void) { return v_one / v_zero; }
static float F_NAN(void) { return v_zero / v_zero; }

TC_CASE(one) { volatile float x = 1.0f; CHECK_BITS_F(log10f(x), 0.0f); }
TC_CASE(ten_hundred) { volatile float a = 10.0f, b = 100.0f; CHECK_NEAR(log10f(a), 1.0, 1e-6); CHECK_NEAR(log10f(b), 2.0, 1e-6); }
TC_CASE(thousand) { volatile float x = 1000.0f; CHECK_NEAR(log10f(x), 3.0, 1e-6); }
TC_CASE(tenth) { volatile float x = 0.1f; CHECK_NEAR(log10f(x), -1.0, 1e-6); }
TC_CASE(inf) { volatile float i = F_INF(); CHECK_BITS_F(log10f(i), i); }

int main(void) { tc_begin(); TC_LOAD_PROC(log10f); if (!tc_p_log10f) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(one); TC_RUN(ten_hundred); TC_RUN(thousand); TC_RUN(tenth); TC_RUN(inf); return tc_end(); }
