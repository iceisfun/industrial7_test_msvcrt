/* TC
 * id:    m99_log2_exp2_basic
 * area:  math_c99
 * funcs: log2 log2f exp2 exp2f
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.7.2 7.12.6.4; MS docs log2, exp2
 * note:  results for powers of two are checked with tolerance; exactness is probed in m99_log2_exp2_exact_probe
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
#define PINF (v_one / v_zero)
#define QNAN (PINF - PINF)
#define FPINF (f_one / f_zero)
#define FQNAN (FPINF - FPINF)

TC_CASE(log2_exact) { volatile double a = 1, b = 2, c = 8, d = 1024, e = 0.5;
    CHECK_BITS_D(log2(a), 0.0); CHECK_NEAR(log2(b), 1.0, 1e-15); CHECK_NEAR(log2(c), 3.0, 1e-15); CHECK_NEAR(log2(d), 10.0, 1e-14); CHECK_NEAR(log2(e), -1.0, 1e-15); }
TC_CASE(exp2_exact) { volatile double a = 0, b = 10, c = -1, d = 3;
    CHECK_BITS_D(exp2(a), 1.0); CHECK_NEAR(exp2(b), 1024.0, 1e-9); CHECK_NEAR(exp2(c), 0.5, 1e-15); CHECK_NEAR(exp2(d), 8.0, 1e-12); }
TC_CASE(exp2_half) { volatile double a = 0.5; CHECK_NEAR(exp2(a), 1.4142135623730951, 1e-15); }
TC_CASE(float_variants) { volatile float a = 8.0f, b = 4.0f; CHECK_NEAR(log2f(a), 3.0, 1e-6); CHECK_NEAR(exp2f(b), 16.0, 1e-4); }

int main(void) { tc_begin(); TC_RUN(log2_exact); TC_RUN(exp2_exact); TC_RUN(exp2_half); TC_RUN(float_variants); return tc_end(); }
