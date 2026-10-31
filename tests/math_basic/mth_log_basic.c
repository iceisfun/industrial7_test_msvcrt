/* TC
 * id:    mth_log_basic
 * area:  math_basic
 * funcs:
 * procs: log
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.6.7, F.10.3.7; MS docs log
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(double, log, (double));
#define log tc_p_log

static volatile double v_zero = 0.0, v_one = 1.0, v_inf, v_nan;
static void init(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

TC_CASE(one) { volatile double x = 1.0; CHECK_BITS_D(log(x), 0.0); }
TC_CASE(e) { volatile double x = 2.718281828459045; CHECK_NEAR(log(x), 1.0, 1e-15); }
TC_CASE(ten) { volatile double x = 10.0; CHECK_NEAR(log(x), 2.302585092994046, 1e-14); }
TC_CASE(half) { volatile double x = 0.5; CHECK_NEAR(log(x), -0.6931471805599453, 1e-15); }
TC_CASE(inf) { volatile double x = v_inf; CHECK(log(x) == v_inf); }
TC_CASE(nan) { volatile double x = v_nan; CHECK_IS_NAN(log(x)); }

int main(void) { tc_begin(); TC_LOAD_PROC(log); init(); TC_RUN(one); TC_RUN(e); TC_RUN(ten); TC_RUN(half); TC_RUN(inf); TC_RUN(nan); return tc_end(); }
