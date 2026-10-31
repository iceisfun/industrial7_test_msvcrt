/* TC
 * id:    m99_expm1_log1p_special
 * area:  math_c99
 * funcs: expm1 log1p expm1f log1pf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 F.10.3.4, F.10.4.6
 * note:  
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
#define DINF (v_one / v_zero)
#define FINF (f_one / f_zero)

TC_CASE(expm1_zero) { volatile double p = 0.0, n = -0.0; CHECK_BITS_D(expm1(p), 0.0); CHECK_BITS_D(expm1(n), -0.0); }
TC_CASE(expm1_inf) { volatile double i = DINF; CHECK_BITS_D(expm1(-i), -1.0); CHECK_BITS_D(expm1(i), i); }
TC_CASE(log1p_zero) { volatile double p = 0.0, n = -0.0; CHECK_BITS_D(log1p(p), 0.0); CHECK_BITS_D(log1p(n), -0.0); }
TC_CASE(log1p_pole_inf) { volatile double m = -1.0, i = DINF; CHECK_BITS_D(log1p(m), -i); CHECK_BITS_D(log1p(i), i); }
TC_CASE(nan_in) { volatile double n = DINF - DINF; CHECK_IS_NAN(expm1(n)); CHECK_IS_NAN(log1p(n)); }
TC_CASE(float_special) { volatile float p = 0.0f, n = -0.0f, i = FINF, m = -1.0f, q = FINF - FINF;
    CHECK_BITS_F(expm1f(n), -0.0f); CHECK_BITS_F(expm1f(p), 0.0f); CHECK_BITS_F(expm1f(-i), -1.0f); CHECK_BITS_F(expm1f(i), i);
    CHECK_BITS_F(log1pf(n), -0.0f); CHECK_BITS_F(log1pf(m), -i); CHECK_BITS_F(log1pf(i), i);
    CHECK(expm1f(q) != expm1f(q)); CHECK(log1pf(q) != log1pf(q)); }

int main(void) { tc_begin(); TC_RUN(expm1_zero); TC_RUN(expm1_inf); TC_RUN(log1p_zero); TC_RUN(log1p_pole_inf); TC_RUN(nan_in); TC_RUN(float_special); return tc_end(); }
