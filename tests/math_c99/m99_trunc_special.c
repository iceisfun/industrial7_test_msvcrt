/* TC
 * id:    m99_trunc_special
 * area:  math_c99
 * funcs: trunc truncf
 * procs: nextafter
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.6.8
 */
#include "tc.h"
#include <math.h>

TC_DECL_PROC(double, nextafter, (double, double));
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;

TC_CASE(zeros) { volatile double p = 0.0, n = -0.0; volatile float fp = 0.0f, fn = -0.0f;
    CHECK_BITS_D(trunc(p), 0.0); CHECK_BITS_D(trunc(n), -0.0); CHECK_BITS_F(truncf(fp), 0.0f); CHECK_BITS_F(truncf(fn), -0.0f); }
TC_CASE(infs) { volatile double i = v_one / v_zero; volatile float fi = f_one / f_zero;
    CHECK_BITS_D(trunc(i), i); CHECK_BITS_D(trunc(-i), -i); CHECK_BITS_F(truncf(fi), fi); CHECK_BITS_F(truncf(-fi), -fi); }
TC_CASE(nans) { volatile double i = v_one / v_zero; volatile double n = i - i; volatile float fi = f_one / f_zero; volatile float fn = fi - fi;
    CHECK_IS_NAN(trunc(n)); CHECK_IS_NAN(truncf(fn)); }
TC_CASE(below_one) { volatile double one = 1.0, zero = 0.0; double p; if (!tc_p_nextafter) TC_SKIP("no nextafter"); p = tc_p_nextafter(one, zero); CHECK(p < 1.0); CHECK_BITS_D(trunc(p), 0.0); }

int main(void) { tc_begin(); TC_LOAD_PROC(nextafter); TC_RUN(zeros); TC_RUN(infs); TC_RUN(nans); TC_RUN(below_one); return tc_end(); }
