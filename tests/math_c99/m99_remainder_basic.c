/* TC
 * id:    m99_remainder_basic
 * area:  math_c99
 * funcs: remainder remainderf
 * procs: 
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.12.10.2; MS docs remainder
 * note:  IEEE remainder: quotient rounded to nearest, ties to even
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
static volatile double v_x, v_y; static volatile float v_f, v_g;
TC_CASE(ties) { v_x = 5; v_y = 3; CHECK_BITS_D(remainder(v_x, v_y), -1.0); v_x = 7; v_y = 2; CHECK_BITS_D(remainder(v_x, v_y), -1.0); v_x = 5; v_y = 2; CHECK_BITS_D(remainder(v_x, v_y), 1.0); }
TC_CASE(neg) { v_x = -5; v_y = 3; CHECK_BITS_D(remainder(v_x, v_y), 1.0); v_x = 1; v_y = 3; CHECK_BITS_D(remainder(v_x, v_y), 1.0); }
TC_CASE(exact) { v_x = 10; v_y = 5; CHECK_BITS_D(remainder(v_x, v_y), 0.0); }
TC_CASE(float_v) { v_f = 5; v_g = 3; CHECK_BITS_F(remainderf(v_f, v_g), -1.0f); }
TC_CASE(vs_fmod) { v_x = 7; v_y = 4; CHECK_BITS_D(remainder(v_x, v_y), -1.0); CHECK_BITS_D(fmod(v_x, v_y), 3.0); }
int main(void) { tc_begin(); TC_RUN(ties); TC_RUN(neg); TC_RUN(exact); TC_RUN(float_v); TC_RUN(vs_fmod); return tc_end(); }
