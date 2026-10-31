/* TC
 * id:    mf_logf_basic
 * area:  math_float
 * funcs:
 * procs: logf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 F.10.3.11; MS docs log
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

TC_DECL_PROC(float, logf, (float));
#undef logf
#define logf tc_p_logf

static volatile float v_zero = 0.0f, v_one = 1.0f;
static float F_INF(void) { return v_one / v_zero; }
static float F_NAN(void) { return v_zero / v_zero; }

TC_CASE(one) { volatile float x = 1.0f; CHECK_BITS_F(logf(x), 0.0f); }
TC_CASE(inf) { volatile float i = F_INF(); CHECK_BITS_F(logf(i), i); }
TC_CASE(e) { volatile float x = 2.7182817f; CHECK_NEAR(logf(x), 1.0, 1e-6); }
TC_CASE(half) { volatile float x = 0.5f; CHECK_NEAR(logf(x), -0.6931472, 1e-6); }
TC_CASE(tiny) { volatile float x = 1e-30f; CHECK_NEAR(logf(x), -69.07755, 1e-4); }

int main(void) { tc_begin(); TC_LOAD_PROC(logf); if (!tc_p_logf) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(one); TC_RUN(inf); TC_RUN(e); TC_RUN(half); TC_RUN(tiny); return tc_end(); }
