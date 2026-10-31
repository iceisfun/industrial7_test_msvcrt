/* TC
 * id:    mf_neg_atan2f_nan
 * area:  math_float
 * funcs:
 * procs: atan2f
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.1.4 / 7.12.4.4; MS docs atan2
 * note:  NaN operand yields NaN
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

TC_DECL_PROC(float, atan2f, (float, float));
#undef atan2f
#define atan2f tc_p_atan2f

static volatile float v_zero = 0.0f, v_one = 1.0f;
static float F_INF(void) { return v_one / v_zero; }
static float F_NAN(void) { return v_zero / v_zero; }

TC_CASE(nan_first)  { volatile float y = F_NAN(), x = 1.0f; CHECK_IS_NAN(atan2f(y, x)); }
TC_CASE(nan_second) { volatile float y = 1.0f, x = F_NAN(); CHECK_IS_NAN(atan2f(y, x)); }
TC_CASE(nan_both)   { volatile float y = F_NAN(), x = F_NAN(); CHECK_IS_NAN(atan2f(y, x)); }

int main(void) { tc_begin(); TC_LOAD_PROC(atan2f); if (!tc_p_atan2f) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(nan_first); TC_RUN(nan_second); TC_RUN(nan_both); return tc_end(); }
