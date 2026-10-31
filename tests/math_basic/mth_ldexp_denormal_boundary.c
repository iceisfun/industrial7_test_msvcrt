/* TC
 * id:    mth_ldexp_denormal_boundary
 * area:  math_basic
 * funcs:
 * procs: ldexp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.12.6.6; MS docs ldexp
 * note:  denormal results/inputs are covered separately in mth_ldexp_edge_probe
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
TC_DECL_PROC(double, ldexp, (double, int));
#define ldexp tc_p_ldexp

static volatile double v_zero = 0.0, v_one = 1.0;

static double L(double x, int e) { volatile double vx = x; volatile int ve = e; return ldexp(vx, ve); }
TC_CASE(dbl_min) { CHECK_EQ_U(tc_dbits(L(1.0, -1022)), 0x0010000000000000ULL); }
TC_CASE(max_pow) { CHECK_EQ_U(tc_dbits(L(0.5, 1024)), 0x7FE0000000000000ULL); }

int main(void) { tc_begin(); TC_LOAD_PROC(ldexp); if (!tc_p_ldexp) { tc_write("TC FAIL " TC_TEST_ID "::ldexp not exported\n"); return 1; } TC_RUN(dbl_min); TC_RUN(max_pow); return tc_end(); }
