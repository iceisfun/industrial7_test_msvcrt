/* TC
 * id:    m99_scalbn_special
 * area:  math_c99
 * funcs: scalbn scalbln scalbnf
 * procs: 
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 F.10.3.13
 * note:  Annex F special values
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
static volatile double v_zero = 0.0, v_one = 1.0;
TC_CASE(zeros) { volatile double z = -0.0; CHECK_BITS_D(scalbn(v_zero, 7), 0.0); CHECK_BITS_D(scalbn(z, -7), -0.0); CHECK_BITS_D(scalbln(z, 3), -0.0); }
TC_CASE(infs) { volatile double i = v_one / v_zero; CHECK(scalbn(i, -5) == i); CHECK(scalbn(-i, 5) == -i); }
TC_CASE(nan) { volatile double n = (v_one / v_zero) - (v_one / v_zero); volatile float fn = (float)n; CHECK_IS_NAN(scalbn(n, 3)); CHECK_IS_NAN(scalbnf(fn, 3)); }
TC_CASE(underflow) { volatile double a = 1; CHECK_BITS_D(scalbn(a, -2000), 0.0); }
TC_CASE(ln0) { volatile double a = 5; CHECK_BITS_D(scalbln(a, 0), 5.0); }
int main(void) { tc_begin(); TC_RUN(zeros); TC_RUN(infs); TC_RUN(nan); TC_RUN(underflow); TC_RUN(ln0); return tc_end(); }
