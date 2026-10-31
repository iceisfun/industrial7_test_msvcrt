/* TC
 * id:    mf_neg_expf_overflow_underflow
 * area:  math_float
 * funcs:
 * procs: expf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs exp (ERANGE on overflow); C11 F.10.3.1
 * note:  underflow errno deliberately not asserted here (see mf_neg_expf_underflow_errno_probe)
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

TC_CASE(overflow_89) { volatile float x = 89.0f; CHECK_BITS_F(expf(x), F_INF()); }
TC_CASE(overflow_1000) { volatile float x = 1000.0f; CHECK_BITS_F(expf(x), F_INF()); }
TC_CASE(overflow_errno) { volatile float x = 1000.0f; errno = 0; (void)expf(x); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(underflow_zero) { volatile float x = -1000.0f; CHECK_BITS_F(expf(x), 0.0f); }
TC_CASE(underflow_104) { volatile float x = -104.0f; float r = expf(x); CHECK(r >= 0.0f); CHECK(r <= FLT_MIN); }

int main(void) { tc_begin(); TC_LOAD_PROC(expf); if (!tc_p_expf) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(overflow_89); TC_RUN(overflow_1000); TC_RUN(overflow_errno); TC_RUN(underflow_zero); TC_RUN(underflow_104); return tc_end(); }
