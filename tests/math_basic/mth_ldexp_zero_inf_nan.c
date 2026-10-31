/* TC
 * id:    mth_ldexp_zero_inf_nan
 * area:  math_basic
 * funcs:
 * procs: ldexp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 F.10.3.8; MS docs ldexp
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
TC_DECL_PROC(double, ldexp, (double, int));
#define ldexp tc_p_ldexp

static volatile double v_zero = 0.0, v_one = 1.0;

static double L(double x, int e) { volatile double vx = x; volatile int ve = e; return ldexp(vx, ve); }
TC_CASE(pzero) { CHECK_BITS_D(L(0.0, 100), 0.0); }
TC_CASE(pinf) { double inf = v_one / v_zero; CHECK(L(inf, -5) == inf); }
TC_CASE(ninf) { double inf = v_one / v_zero; CHECK(L(-inf, 5) == -inf); }
TC_CASE(nan_in) { CHECK_IS_NAN(L(v_zero / v_zero, 3)); }

int main(void) { tc_begin(); TC_LOAD_PROC(ldexp); if (!tc_p_ldexp) { tc_write("TC FAIL " TC_TEST_ID "::ldexp not exported\n"); return 1; } TC_RUN(pzero); TC_RUN(pinf); TC_RUN(ninf); TC_RUN(nan_in); return tc_end(); }
