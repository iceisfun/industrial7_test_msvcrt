/* TC
 * id:    m99_remquo_neg_domain
 * area:  math_c99
 * funcs: remquo remquof
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.10.3, F.10.7.3; MS docs remquo
 * note:  The *quo value is unspecified on domain error and not asserted; errno not asserted.
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
static float f_nan(void) { return f_inf() - f_inf(); }

TC_CASE(by_zero) { volatile double x = 1.0, z = 0.0; int q = 0; CHECK_IS_NAN(remquo(x, z, &q)); }
TC_CASE(inf_dividend) { volatile double t = 3.0; double i = d_inf(); int q = 0; CHECK_IS_NAN(remquo(i, t, &q)); CHECK_IS_NAN(remquo(-i, t, &q)); }
TC_CASE(nan_dividend) { volatile double t = 3.0; double n = d_nan(); int q = 0; CHECK_IS_NAN(remquo(n, t, &q)); }
TC_CASE(float_by_zero) { volatile float x = 1.0f, z = 0.0f; int q = 0; float r = remquof(x, z, &q); CHECK(r != r); }
TC_CASE(float_inf_dividend) { volatile float t = 3.0f; float i = f_inf(), r; int q = 0; r = remquof(i, t, &q); CHECK(r != r); }
int main(void) { tc_begin(); TC_RUN(by_zero); TC_RUN(inf_dividend); TC_RUN(nan_dividend); TC_RUN(float_by_zero); TC_RUN(float_inf_dividend); return tc_end(); }
