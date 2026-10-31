/* TC
 * id:    m99_round_basic
 * area:  math_c99
 * funcs: round roundf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.9.6; MS docs round
 */
#include "tc.h"
#include <math.h>

TC_CASE(half_away) { volatile double a = 0.5, b = 1.5, c = 2.5; CHECK_BITS_D(round(a), 1.0); CHECK_BITS_D(round(b), 2.0); CHECK_BITS_D(round(c), 3.0); }
TC_CASE(negative) { volatile double a = -0.5, b = -2.5; CHECK_BITS_D(round(a), -1.0); CHECK_BITS_D(round(b), -3.0); }
TC_CASE(floats) { volatile float a = 2.5f, b = -2.5f; CHECK_BITS_F(roundf(a), 3.0f); CHECK_BITS_F(roundf(b), -3.0f); }
TC_CASE(neg_zero_result) { volatile double a = -0.3; CHECK_BITS_D(round(a), -0.0); }

int main(void) { tc_begin(); TC_RUN(half_away); TC_RUN(negative); TC_RUN(floats); TC_RUN(neg_zero_result); return tc_end(); }
