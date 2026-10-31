/* TC
 * id:    mf_asinf_acosf_basic
 * area:  math_float
 * funcs:
 * procs: asinf acosf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 F.10.1.2/F.10.1.1; MS docs asin, acos
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(float, asinf, (float));
#undef asinf
#define asinf tc_p_asinf
TC_DECL_PROC(float, acosf, (float));
#undef acosf
#define acosf tc_p_acosf

#define PI2 1.5707963267948966
TC_CASE(asin_zero) { volatile float p = 0.0f, n = -0.0f; CHECK_BITS_F(asinf(p), 0.0f); CHECK_BITS_F(asinf(n), -0.0f); }
TC_CASE(acos_one) { volatile float x = 1.0f; CHECK_BITS_F(acosf(x), 0.0f); }
TC_CASE(asin_pm1) { volatile float a = 1.0f, b = -1.0f; CHECK_NEAR(asinf(a), PI2, 1e-6); CHECK_NEAR(asinf(b), -PI2, 1e-6); }
TC_CASE(acos_0_m1) { volatile float a = 0.0f, b = -1.0f; CHECK_NEAR(acosf(a), PI2, 1e-6); CHECK_NEAR(acosf(b), 3.14159265358979, 1e-6); }
TC_CASE(asin_half) { volatile float x = 0.5f; CHECK_NEAR(asinf(x), 0.5235988, 1e-6); }
int main(void) { tc_begin(); TC_LOAD_PROC(asinf); TC_LOAD_PROC(acosf); if (!(tc_p_asinf && tc_p_acosf)) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(asin_zero); TC_RUN(acos_one); TC_RUN(asin_pm1); TC_RUN(acos_0_m1); TC_RUN(asin_half); return tc_end(); }
