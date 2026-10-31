/* TC
 * id:    m99_atanh_basic
 * area:  math_c99
 * funcs: atanh atanhf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.12.5.3, F.10.2.3; MS docs atanh

 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
static double d_inf(void) { return v_one / v_zero; }
static double d_nan(void) { return d_inf() - d_inf(); }
static float f_inf(void) { return f_one / f_zero; }

TC_CASE(zeros) { CHECK_BITS_D(atanh(v_zero), 0.0); CHECK_BITS_D(atanh(-v_zero * v_one), -0.0 * v_one); }
TC_CASE(half) { volatile double x = 0.5; CHECK_NEAR(atanh(x), 0.5493061443340548, 1e-15); CHECK_BITS_D(atanh(-x), -atanh(x)); }
TC_CASE(tiny) { volatile double x = 1e-10; CHECK_NEAR(atanh(x), 1e-10, 1e-25); }
TC_CASE(floatv) { volatile float x = 0.5f; CHECK_NEAR(atanhf(x), 0.54930615f, 1e-6); }
int main(void) { tc_begin(); TC_RUN(zeros); TC_RUN(half); TC_RUN(tiny); TC_RUN(floatv); return tc_end(); }
