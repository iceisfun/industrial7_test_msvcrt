/* TC
 * id:    m99_remquo_quo_zero_probe
 * area:  math_c99
 * funcs: remquo remquof
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.12.10.3
 * note:  PROBE: *quo is written (as 0, the low bits of an integral quotient of 0) when x is finite and y is +-inf, or x is +-0.
 *        The standard implies this; the implementation might return early without storing. q is preset to 99.
 */
#include "tc.h"
#include <math.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
static double d_inf(void) { return v_one / v_zero; }
static float f_inf(void) { return f_one / f_zero; }

TC_CASE(finite_by_inf) { volatile double x = 1.0; double i = d_inf(); int q = 99; remquo(x, i, &q); CHECK_EQ_I(q, 0); q = 99; remquo(x, -i, &q); CHECK_EQ_I(q, 0); }
TC_CASE(float_finite_by_inf) { volatile float x = 1.0f; float i = f_inf(); int q = 99; remquof(x, i, &q); CHECK_EQ_I(q, 0); }
TC_CASE(zero_dividend) { volatile double pz = 0.0, nz = -0.0, y = 5.0; int q = 99; remquo(pz, y, &q); CHECK_EQ_I(q, 0); q = 99; remquo(nz, y, &q); CHECK_EQ_I(q, 0); }
int main(void) { tc_begin(); TC_RUN(finite_by_inf); TC_RUN(float_finite_by_inf); TC_RUN(zero_dividend); return tc_end(); }
