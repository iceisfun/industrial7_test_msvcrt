/* TC
 * id:    mf_neg_logf_errno
 * area:  math_float
 * funcs:
 * procs: logf log10f
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs log, log10 (EDOM for negative, ERANGE for zero); C11 7.12.1
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

TC_DECL_PROC(float, logf, (float));
#undef logf
#define logf tc_p_logf
TC_DECL_PROC(float, log10f, (float));
#undef log10f
#define log10f tc_p_log10f

static volatile float v_zero = 0.0f, v_one = 1.0f;
static float F_INF(void) { return v_one / v_zero; }
static float F_NAN(void) { return v_zero / v_zero; }

TC_CASE(logf_neg_edom) { volatile float x = -1.0f; errno = 0; (void)logf(x); CHECK_EQ_I(errno, EDOM); }
TC_CASE(log10f_neg_edom) { volatile float x = -1.0f; errno = 0; (void)log10f(x); CHECK_EQ_I(errno, EDOM); }
TC_CASE(logf_zero_erange) { volatile float x = 0.0f; errno = 0; (void)logf(x); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(log10f_zero_erange) { volatile float x = 0.0f; errno = 0; (void)log10f(x); CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); TC_LOAD_PROC(logf); TC_LOAD_PROC(log10f); if (!tc_p_logf || !tc_p_log10f) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(logf_neg_edom); TC_RUN(log10f_neg_edom); TC_RUN(logf_zero_erange); TC_RUN(log10f_zero_erange); return tc_end(); }
