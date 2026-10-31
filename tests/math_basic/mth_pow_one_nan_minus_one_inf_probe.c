/* TC
 * id:    mth_pow_one_nan_minus_one_inf_probe
 * area:  math_basic
 * funcs:
 * procs: pow
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   C11 F.10.4.4
 * note:  probing C99 special cases pow(+1, NaN) == 1 and pow(-1, +-inf) == 1 on Windows 10 ucrtbase; the MS docs do not
 *        list them explicitly and early UCRT builds may have differed
 */
#include "tc.h"
#include <math.h>

TC_DECL_PROC(double, pow, (double, double));
#define pow tc_p_pow

static volatile double v_zero = 0.0, v_one = 1.0, v_inf, v_nan;
static void init(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

TC_CASE(one_to_nan) { volatile double x = 1.0, y = v_nan; CHECK_BITS_D(pow(x, y), 1.0); }
TC_CASE(minus_one_inf) { volatile double x = -1.0, y = v_inf, z = -v_inf; CHECK_BITS_D(pow(x, y), 1.0); CHECK_BITS_D(pow(x, z), 1.0); }

int main(void) { tc_begin(); TC_LOAD_PROC(pow); init(); TC_RUN(one_to_nan); TC_RUN(minus_one_inf); return tc_end(); }
