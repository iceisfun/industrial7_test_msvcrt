/* TC
 * id:    m99_expm1_log1p_basic
 * area:  math_c99
 * funcs: expm1 expm1f log1p log1pf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.12.6.3, 7.12.6.9; MS docs expm1, log1p
 * note:  relative epsilons stated per case
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
#define DINF (v_one / v_zero)
#define FINF (f_one / f_zero)

TC_CASE(zeros) { volatile double z = 0.0; CHECK_BITS_D(expm1(z), 0.0); CHECK_BITS_D(log1p(z), 0.0); }
TC_CASE(expm1_small) { volatile double x = 1e-10; double r = expm1(x); CHECK_NEAR(r, 1.00000000005e-10, 1e-10 * 1e-9); }
TC_CASE(log1p_small) { volatile double x = 1e-10; double r = log1p(x); CHECK_NEAR(r, 9.9999999995e-11, 1e-10 * 1e-9); }
TC_CASE(expm1_one) { volatile double x = 1.0; CHECK_NEAR(expm1(x), 1.718281828459045, 1e-14); }
TC_CASE(log1p_em1) { volatile double x = 1.718281828459045; CHECK_NEAR(log1p(x), 1.0, 1e-14); }
TC_CASE(float_funcs) { volatile float a = 1e-5f, b = 1.0f; CHECK_NEAR(expm1f(a), 1.0000050e-5, 1e-10); CHECK_NEAR(log1pf(b), 0.6931472, 1e-6); }

int main(void) { tc_begin(); TC_RUN(zeros); TC_RUN(expm1_small); TC_RUN(log1p_small); TC_RUN(expm1_one); TC_RUN(log1p_em1); TC_RUN(float_funcs); return tc_end(); }
