/* TC
 * id:    m99_lrint_default_mode
 * area:  math_c99
 * funcs: lrint lrintf llrint llrintf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.9.5; MS docs lrint, llrint
 */
#include "tc.h"
#include <math.h>
#include <limits.h>

TC_CASE(lrint_vals) { volatile double a = 0.5, b = 1.5, c = 2.5, d = -2.5, e = -0.5;
    CHECK_EQ_I(lrint(a), 0); CHECK_EQ_I(lrint(b), 2); CHECK_EQ_I(lrint(c), 2); CHECK_EQ_I(lrint(d), -2); CHECK_EQ_I(lrint(e), 0); }
TC_CASE(llrint_vals) { volatile double a = 3.5, b = -3.5; CHECK_EQ_I(llrint(a), 4); CHECK_EQ_I(llrint(b), -4); }
TC_CASE(floats) { volatile float a = 2.5f, b = 0.5f; CHECK_EQ_I(lrintf(a), 2); CHECK_EQ_I(llrintf(b), 0); }
TC_CASE(int_max) { volatile double a = 2147483647.0; CHECK_EQ_I(lrint(a), INT_MAX); }

int main(void) { tc_begin(); TC_RUN(lrint_vals); TC_RUN(llrint_vals); TC_RUN(floats); TC_RUN(int_max); return tc_end(); }
