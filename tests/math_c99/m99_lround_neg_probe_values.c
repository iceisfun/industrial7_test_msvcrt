/* TC
 * id:    m99_lround_neg_probe_values
 * area:  math_c99
 * funcs: lround llround lrint llrint
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   x86 integer-indefinite convention
 * note:  A: observed on Win10 19045 ucrtbase (raw: lround(nan)=lround(inf)=llround(1e30)=lrint(1e10)=llrint(nan)=0): out-of-range / NaN / Inf conversions return 0, NOT the x86 integer indefinite LONG_MIN/LLONG_MIN. C11 7.12.9.x: result unspecified, so this is implementation behaviour (stable on this build; may depend on ucrtbase version). Was PROBE: whether out-of-range/NaN/Inf conversions yield LONG_MIN / LLONG_MIN (0x80000000 / 0x8000000000000000) on Windows. Observed on an FMA3/AVX2 CPU (ucrtbase dispatches FMA3 paths); non-FMA path unverified.
 */
#include "tc.h"
#include <math.h>
#include <limits.h>

static volatile double v_zero = 0.0, v_one = 1.0;

TC_CASE(lround_nan) { volatile double i = v_one / v_zero; volatile double n = i - i; CHECK_EQ_I(lround(n), 0); }
TC_CASE(lround_inf) { volatile double i = v_one / v_zero; CHECK_EQ_I(lround(i), 0); }
TC_CASE(llround_big) { volatile double a = 1e30; CHECK_EQ_I(llround(a), 0); }
TC_CASE(lrint_big) { volatile double a = 1e10; CHECK_EQ_I(lrint(a), 0); }
TC_CASE(llrint_nan) { volatile double i = v_one / v_zero; volatile double n = i - i; CHECK_EQ_I(llrint(n), 0); }

int main(void) { tc_begin(); TC_RUN(lround_nan); TC_RUN(lround_inf); TC_RUN(llround_big); TC_RUN(lrint_big); TC_RUN(llrint_nan); return tc_end(); }
