/* TC
 * id:    mf_expf_basic
 * area:  math_float
 * funcs:
 * procs: expf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 F.10.3.1; MS docs exp
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

TC_DECL_PROC(float, expf, (float));
#undef expf
#define expf tc_p_expf

static volatile float v_zero = 0.0f, v_one = 1.0f;
static float F_INF(void) { return v_one / v_zero; }
static float F_NAN(void) { return v_zero / v_zero; }

TC_CASE(zero) { volatile float x = 0.0f; CHECK_BITS_F(expf(x), 1.0f); }
TC_CASE(inf) { volatile float i = F_INF(), n = -F_INF(); CHECK_BITS_F(expf(n), 0.0f); CHECK_BITS_F(expf(i), i); }
TC_CASE(one) { volatile float x = 1.0f, y = -1.0f; CHECK_NEAR(expf(x), 2.7182817, 1e-6); CHECK_NEAR(expf(y), 0.36787945, 1e-6); }
TC_CASE(large_finite) { volatile float x = 88.0f; float r = expf(x); CHECK(r > 1e38f); CHECK(r < FLT_MAX); }
TC_CASE(nan) { volatile float x = F_NAN(); CHECK_IS_NAN(expf(x)); }

int main(void) { tc_begin(); TC_LOAD_PROC(expf); if (!tc_p_expf) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(zero); TC_RUN(inf); TC_RUN(one); TC_RUN(large_finite); TC_RUN(nan); return tc_end(); }
