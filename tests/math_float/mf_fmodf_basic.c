/* TC
 * id:    mf_fmodf_basic
 * area:  math_float
 * funcs: fmodf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.7.1; MS docs fmod, fmodf
 * note:  sign-of-zero results (fmodf(-0,y), fmodf(-6,3)) are in mf_fmodf_signed_zero_probe
 */
#include "tc.h"
#include <math.h>

static volatile float v_zero = 0.0f, v_one = 1.0f;

TC_CASE(frac) { volatile float a = 5.5f, b = -5.5f, two = 2.0f, m2 = -2.0f; CHECK_BITS_F(fmodf(a, two), 1.5f); CHECK_BITS_F(fmodf(b, two), -1.5f); CHECK_BITS_F(fmodf(a, m2), 1.5f); }
TC_CASE(zero_x) { volatile float p = 0.0f, t = 3.0f; CHECK_BITS_F(fmodf(p, t), 0.0f); }
TC_CASE(inf_y) { volatile float x = 3.0f, y = v_one / v_zero; CHECK_BITS_F(fmodf(x, y), 3.0f); }
TC_CASE(x_lt_y) { volatile float x = 1.0f, y = 3.0f; CHECK_BITS_F(fmodf(x, y), 1.0f); }
TC_CASE(exact_multiple) { volatile float a = 6.0f, t = 3.0f; CHECK_BITS_F(fmodf(a, t), 0.0f); }

int main(void) { tc_begin(); TC_RUN(frac); TC_RUN(zero_x); TC_RUN(inf_y); TC_RUN(x_lt_y); TC_RUN(exact_multiple); return tc_end(); }
