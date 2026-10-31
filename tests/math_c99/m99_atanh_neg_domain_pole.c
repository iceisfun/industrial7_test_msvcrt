/* TC
 * id:    m99_atanh_neg_domain_pole
 * area:  math_c99
 * funcs: atanh atanhf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.2.3 (|x|>1 domain error NaN; +-1 returns +-inf, divide-by-zero); MS docs atanh

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

TC_CASE(domain) { volatile double a = 2.0, b = -2.0; CHECK_IS_NAN(atanh(a)); CHECK_IS_NAN(atanh(b)); CHECK_IS_NAN(atanh(d_inf())); }
TC_CASE(poles) { volatile double a = 1.0, b = -1.0; CHECK_BITS_D(atanh(a), d_inf()); CHECK_BITS_D(atanh(b), -d_inf()); }
TC_CASE(nan) { CHECK_IS_NAN(atanh(d_nan())); }
TC_CASE(floatv) { volatile float a = 1.5f, b = 1.0f; float r = atanhf(a); CHECK(r != r); CHECK_BITS_F(atanhf(b), f_inf()); }
int main(void) { tc_begin(); TC_RUN(domain); TC_RUN(poles); TC_RUN(nan); TC_RUN(floatv); return tc_end(); }
