/* TC
 * id:    mth_log10_pow10_exact_probe
 * area:  math_basic
 * funcs: log10
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   C11 F.10.3.8 (exact results only recommended)
 * note:  probing exactness of log10(10^n)==n for n=1..15 and 22; commonly exact but unspecified
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0, v_inf, v_nan;
static void init(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

TC_CASE(pow10_1_to_15) { volatile double x = 1.0; int n; for (n = 1; n <= 15; n++) { x *= 10.0; CHECK_BITS_D(log10(x), (double)n); } }
TC_CASE(pow10_22) { volatile double x = 1e22; CHECK_BITS_D(log10(x), 22.0); }

int main(void) { tc_begin(); init(); TC_RUN(pow10_1_to_15); TC_RUN(pow10_22); return tc_end(); }
