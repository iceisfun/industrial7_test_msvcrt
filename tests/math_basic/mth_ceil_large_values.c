/* TC
 * id:    mth_ceil_large_values
 * area:  math_basic
 * funcs: ceil
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.9.1, F.10.6.1
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0, v_inf, v_nan;
static void init(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

TC_CASE(half) { volatile double a = 0.5; CHECK_BITS_D(ceil(a), 1.0); }
TC_CASE(neg_just_above_minus1) { volatile double a = -0.9999999999999999; CHECK_BITS_D(ceil(a), -0.0); }
TC_CASE(tiny_pos) { volatile double a = 1e-300; CHECK_BITS_D(ceil(a), 1.0); }
TC_CASE(huge) { volatile double a = 1e300, b = -1e300; CHECK_BITS_D(ceil(a), 1e300); CHECK_BITS_D(ceil(b), -1e300); }
TC_CASE(neg_denorm_min) { volatile double a = -4.9406564584124654e-324; CHECK_BITS_D(ceil(a), -0.0); }

int main(void) { tc_begin(); init(); TC_RUN(half); TC_RUN(neg_just_above_minus1); TC_RUN(tiny_pos); TC_RUN(huge); TC_RUN(neg_denorm_min); return tc_end(); }
