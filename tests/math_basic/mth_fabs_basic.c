/* TC
 * id:    mth_fabs_basic
 * area:  math_basic
 * funcs:
 * procs: fabs
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.7.2, F.10.4.3; MS docs fabs
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(double, fabs, (double));
#define fabs tc_p_fabs

static volatile double v_zero = 0.0, v_one = 1.0, v_inf, v_nan;
static void init(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

TC_CASE(basic) { volatile double a = -3.5, b = 3.5; CHECK_BITS_D(fabs(a), 3.5); CHECK_BITS_D(fabs(b), 3.5); }
TC_CASE(neg_zero) { volatile double a = -0.0; CHECK_BITS_D(fabs(a), 0.0); }
TC_CASE(inf) { volatile double a = -v_inf; CHECK_BITS_D(fabs(a), v_inf); }
TC_CASE(nan_sign_clear) { volatile double a = v_nan; double r = fabs(a); CHECK_IS_NAN(r); CHECK((tc_dbits(r) >> 63) == 0); }
TC_CASE(denorm_min) { volatile double a = 4.9406564584124654e-324, b = -4.9406564584124654e-324; CHECK_EQ_U(tc_dbits(fabs(a)), 1); CHECK_EQ_U(tc_dbits(fabs(b)), 1); }

int main(void) { tc_begin(); TC_LOAD_PROC(fabs); init(); TC_RUN(basic); TC_RUN(neg_zero); TC_RUN(inf); TC_RUN(nan_sign_clear); TC_RUN(denorm_min); return tc_end(); }
