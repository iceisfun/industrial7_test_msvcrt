/* TC
 * id:    mth_floor_large_values
 * area:  math_basic
 * funcs: floor
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.9.2, F.10.6.2
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0, v_inf, v_nan;
static void init(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

TC_CASE(near_2_52) { volatile double a = 4503599627370497.0; CHECK_BITS_D(floor(a), 4503599627370497.0); }
TC_CASE(neg_half) { volatile double a = -0.5; CHECK_BITS_D(floor(a), -1.0); }
TC_CASE(just_below_one) { volatile double a = 0.9999999999999999; CHECK_BITS_D(floor(a), 0.0); }
TC_CASE(tiny_neg) { volatile double a = -1e-300; CHECK_BITS_D(floor(a), -1.0); }
TC_CASE(huge) { volatile double a = 1e300; CHECK_BITS_D(floor(a), 1e300); }
TC_CASE(denorm_min) { volatile double a = 4.9406564584124654e-324; CHECK_BITS_D(floor(a), 0.0); }

int main(void) { tc_begin(); init(); TC_RUN(near_2_52); TC_RUN(neg_half); TC_RUN(just_below_one); TC_RUN(tiny_neg); TC_RUN(huge); TC_RUN(denorm_min); return tc_end(); }
