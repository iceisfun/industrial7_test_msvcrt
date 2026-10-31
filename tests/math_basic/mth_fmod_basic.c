/* TC
 * id:    mth_fmod_basic
 * area:  math_basic
 * funcs: fmod
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.10.1, F.10.7.1; MS docs fmod
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0, v_inf, v_nan;
static void init(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

TC_CASE(signs) { volatile double a = 5.5, b = -5.5, p = 2.0, n = -2.0; CHECK_BITS_D(fmod(a, p), 1.5); CHECK_BITS_D(fmod(b, p), -1.5); CHECK_BITS_D(fmod(a, n), 1.5); }
TC_CASE(inf_divisor) { volatile double a = 1.0, b = -3.0, i = v_inf; CHECK_BITS_D(fmod(a, i), 1.0); CHECK_BITS_D(fmod(b, i), -3.0); }

int main(void) { tc_begin(); init(); TC_RUN(signs); TC_RUN(inf_divisor); return tc_end(); }
