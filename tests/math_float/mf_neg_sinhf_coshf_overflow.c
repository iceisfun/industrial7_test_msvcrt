/* TC
 * id:    mf_neg_sinhf_coshf_overflow
 * area:  math_float
 * funcs:
 * procs: sinhf coshf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sinh, cosh (ERANGE on overflow); C11 F.10.2.5
 * note:  ERANGE asserted in separate cases from the sign/inf checks
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

TC_CASE(sinh_pos) { volatile float x = 100.0f; CHECK_BITS_F(sinhf(x), F_INF()); }
TC_CASE(sinh_neg) { volatile float x = -100.0f; CHECK_BITS_F(sinhf(x), -F_INF()); }
TC_CASE(cosh_pos) { volatile float x = 100.0f; CHECK_BITS_F(coshf(x), F_INF()); }
TC_CASE(cosh_neg) { volatile float x = -100.0f; CHECK_BITS_F(coshf(x), F_INF()); }
TC_CASE(errno_sinh) { volatile float x = 100.0f, y = -100.0f; errno = 0; (void)sinhf(x); CHECK_EQ_I(errno, ERANGE); errno = 0; (void)sinhf(y); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(errno_cosh) { volatile float x = 100.0f, y = -100.0f; errno = 0; (void)coshf(x); CHECK_EQ_I(errno, ERANGE); errno = 0; (void)coshf(y); CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); TC_LOAD_PROC(sinhf); TC_LOAD_PROC(coshf); if (!tc_p_sinhf || !tc_p_coshf) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(sinh_pos); TC_RUN(sinh_neg); TC_RUN(cosh_pos); TC_RUN(cosh_neg); TC_RUN(errno_sinh); TC_RUN(errno_cosh); return tc_end(); }
