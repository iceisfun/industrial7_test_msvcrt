/* TC
 * id:    mth_frexp_basic
 * area:  math_basic
 * funcs:
 * procs: frexp ldexp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.12.6.4; MS docs frexp
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
TC_DECL_PROC(double, frexp, (double, int *));
#define frexp tc_p_frexp
TC_DECL_PROC(double, ldexp, (double, int));
#define ldexp tc_p_ldexp

static volatile double v_zero = 0.0, v_one = 1.0;

static void t(double x, double m, int e) { int ex = 12345; volatile double vx = x; double r = frexp(vx, &ex); CHECK_BITS_D(r, m); CHECK_EQ_I(ex, e); }
TC_CASE(eight) { t(8.0, 0.5, 4); }
TC_CASE(one) { t(1.0, 0.5, 1); }
TC_CASE(neg3) { t(-3.0, -0.75, 2); }
TC_CASE(tenth) { int ex = 0; volatile double x = 0.1; double r = frexp(x, &ex); CHECK(r >= 0.5 && r < 1.0); CHECK_EQ_I(ex, -3); CHECK_NEAR(ldexp(r, ex), 0.1, 1e-17); }
TC_CASE(k1024) { t(1024.0, 0.5, 11); }
TC_CASE(quarter) { t(0.25, 0.5, -1); }

int main(void) { tc_begin(); TC_LOAD_PROC(frexp); TC_LOAD_PROC(ldexp); if (!tc_p_frexp || !tc_p_ldexp) { tc_write("TC FAIL " TC_TEST_ID "::frexp not exported\n"); return 1; } TC_RUN(eight); TC_RUN(one); TC_RUN(neg3); TC_RUN(tenth); TC_RUN(k1024); TC_RUN(quarter); return tc_end(); }
