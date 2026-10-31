/* TC
 * id:    mth_log10_basic
 * area:  math_basic
 * funcs: log10
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.6.8, F.10.3.8; MS docs log10
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0, v_inf, v_nan;
static void init(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

TC_CASE(one) { volatile double x = 1.0; CHECK_BITS_D(log10(x), 0.0); }
TC_CASE(ten) { volatile double x = 10.0; CHECK_NEAR(log10(x), 1.0, 1e-15); }
TC_CASE(hundred) { volatile double x = 100.0; CHECK_NEAR(log10(x), 2.0, 1e-14); }
TC_CASE(milli) { volatile double x = 1e-3; CHECK_NEAR(log10(x), -3.0, 1e-14); }
TC_CASE(two) { volatile double x = 2.0; CHECK_NEAR(log10(x), 0.3010299956639812, 1e-15); }
TC_CASE(inf) { volatile double x = v_inf; CHECK(log10(x) == v_inf); }

int main(void) { tc_begin(); init(); TC_RUN(one); TC_RUN(ten); TC_RUN(hundred); TC_RUN(milli); TC_RUN(two); TC_RUN(inf); return tc_end(); }
