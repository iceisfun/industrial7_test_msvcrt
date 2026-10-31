/* TC
 * id:    m99_lround_neg_probe_values
 * area:  math_c99
 * funcs: lround llround lrint llrint
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   x86 integer-indefinite convention
 * note:  PROBE: whether out-of-range/NaN/Inf conversions yield LONG_MIN / LLONG_MIN (0x80000000 / 0x8000000000000000) on Windows.
 */
#include "tc.h"
#include <math.h>
#include <limits.h>

static volatile double v_zero = 0.0, v_one = 1.0;

TC_CASE(lround_nan) { volatile double i = v_one / v_zero; volatile double n = i - i; CHECK_EQ_I(lround(n), LONG_MIN); }
TC_CASE(lround_inf) { volatile double i = v_one / v_zero; CHECK_EQ_I(lround(i), LONG_MIN); }
TC_CASE(llround_big) { volatile double a = 1e30; CHECK_EQ_I(llround(a), LLONG_MIN); }
TC_CASE(lrint_big) { volatile double a = 1e10; CHECK_EQ_I(lrint(a), LONG_MIN); }
TC_CASE(llrint_nan) { volatile double i = v_one / v_zero; volatile double n = i - i; CHECK_EQ_I(llrint(n), LLONG_MIN); }

int main(void) { tc_begin(); TC_RUN(lround_nan); TC_RUN(lround_inf); TC_RUN(llround_big); TC_RUN(lrint_big); TC_RUN(llrint_nan); return tc_end(); }
