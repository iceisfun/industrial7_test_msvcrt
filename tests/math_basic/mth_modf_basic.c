/* TC
 * id:    mth_modf_basic
 * area:  math_basic
 * funcs: modf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.12.11.1; MS docs modf
 * note:  sign of zero results is covered separately in mth_modf_signed_zero_probe
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;

TC_CASE(pos) { volatile double x = 3.75; double ip = -99; double f = modf(x, &ip); CHECK_BITS_D(f, 0.75); CHECK_BITS_D(ip, 3.0); }
TC_CASE(neg) { volatile double x = -3.75; double ip = -99; double f = modf(x, &ip); CHECK_BITS_D(f, -0.75); CHECK_BITS_D(ip, -3.0); }
TC_CASE(integral_pos) { volatile double x = 5.0; double ip = -99; double f = modf(x, &ip); CHECK_BITS_D(f, 0.0); CHECK_BITS_D(ip, 5.0); }
TC_CASE(integral_neg) { volatile double x = -5.0; double ip = -99; double f = modf(x, &ip); CHECK_EQ_I(f == 0.0, 1); CHECK_BITS_D(ip, -5.0); }
TC_CASE(small_pos) { volatile double x = 0.25; double ip = -99; double f = modf(x, &ip); CHECK_BITS_D(f, 0.25); CHECK_BITS_D(ip, 0.0); }
TC_CASE(small_neg) { volatile double x = -0.25; double ip = -99; double f = modf(x, &ip); CHECK_BITS_D(f, -0.25); CHECK_EQ_I(ip == 0.0, 1); }

int main(void) { tc_begin(); TC_RUN(pos); TC_RUN(neg); TC_RUN(integral_pos); TC_RUN(integral_neg); TC_RUN(small_pos); TC_RUN(small_neg); return tc_end(); }
