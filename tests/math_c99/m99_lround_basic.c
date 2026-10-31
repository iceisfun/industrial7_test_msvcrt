/* TC
 * id:    m99_lround_basic
 * area:  math_c99
 * funcs: lround lroundf llround llroundf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.9.7; MS docs lround, llround
 * note:  long is 32-bit on LLP64 Windows
 */
#include "tc.h"
#include <math.h>

TC_CASE(lround_vals) { volatile double a = 2.5, b = -2.5, c = 0.4, d = -0.5;
    CHECK_EQ_I(lround(a), 3); CHECK_EQ_I(lround(b), -3); CHECK_EQ_I(lround(c), 0); CHECK_EQ_I(lround(d), -1); }
TC_CASE(llround_big) { volatile double a = 10000000000.5; CHECK_EQ_I(llround(a), 10000000001LL); }
TC_CASE(floats) { volatile float a = 2.5f, b = -1.5f; CHECK_EQ_I(llroundf(a), 3); CHECK_EQ_I(lroundf(b), -2); }
TC_CASE(widths) { volatile double a = 1.0; volatile float f = 1.0f;
    CHECK_EQ_U(sizeof(lround(a)), 4); CHECK_EQ_U(sizeof(lroundf(f)), 4);
    CHECK_EQ_U(sizeof(llround(a)), 8); CHECK_EQ_U(sizeof(llroundf(f)), 8); }

int main(void) { tc_begin(); TC_RUN(lround_vals); TC_RUN(llround_big); TC_RUN(floats); TC_RUN(widths); return tc_end(); }
