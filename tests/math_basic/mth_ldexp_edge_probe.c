/* TC
 * id:    mth_ldexp_edge_probe
 * area:  math_basic
 * funcs:
 * procs: ldexp
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.12.6.6, F.10.3.8
 * note:  PROBE: ldexp keeps the sign of zero (-0.0), and produces/accepts denormals exactly on every CRT incl. legacy ones.
 */
#include "tc.h"
#include <math.h>
TC_DECL_PROC(double, ldexp, (double, int));
#define ldexp tc_p_ldexp

static double L(double x, int e) { volatile double vx = x; volatile int ve = e; return ldexp(vx, ve); }
TC_CASE(nzero) { CHECK_BITS_D(L(-0.0, -100), -0.0); }
TC_CASE(min_denorm) { CHECK_EQ_U(tc_dbits(L(1.0, -1074)), 1ULL); }
TC_CASE(normal_to_denorm) { union { unsigned long long u; double d; } v; v.u = 0x0010000000000000ULL; CHECK_EQ_U(tc_dbits(L(v.d, -52)), 1ULL); }
TC_CASE(denorm_to_normal) { union { unsigned long long u; double d; } v; v.u = 1; CHECK_EQ_U(tc_dbits(L(v.d, 52)), 0x0010000000000000ULL); }

int main(void) { tc_begin(); TC_LOAD_PROC(ldexp); if (!tc_p_ldexp) { tc_write("TC FAIL " TC_TEST_ID "::ldexp not exported\n"); return 1; } TC_RUN(nzero); TC_RUN(min_denorm); TC_RUN(normal_to_denorm); TC_RUN(denorm_to_normal); return tc_end(); }
