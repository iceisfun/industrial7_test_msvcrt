/* TC
 * id:    mth_ldexp_basic
 * area:  math_basic
 * funcs:
 * procs: ldexp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.12.6.6; MS docs ldexp
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
TC_DECL_PROC(double, ldexp, (double, int));
#define ldexp tc_p_ldexp

static volatile double v_zero = 0.0, v_one = 1.0;

static double L(double x, int e) { volatile double vx = x; volatile int ve = e; return ldexp(vx, ve); }
TC_CASE(p10) { CHECK_BITS_D(L(1.0, 10), 1024.0); }
TC_CASE(half) { CHECK_BITS_D(L(0.5, 1), 1.0); }
TC_CASE(down) { CHECK_BITS_D(L(3.0, -1), 1.5); }
TC_CASE(neg) { CHECK_BITS_D(L(-1.0, 3), -8.0); }
TC_CASE(zero_exp) { CHECK_BITS_D(L(1.0, 0), 1.0); }
TC_CASE(big) { CHECK_EQ_U(tc_dbits(L(1.5, 1023)), 0x7FE8000000000000ULL); }

int main(void) { tc_begin(); TC_LOAD_PROC(ldexp); if (!tc_p_ldexp) { tc_write("TC FAIL " TC_TEST_ID "::ldexp not exported\n"); return 1; } TC_RUN(p10); TC_RUN(half); TC_RUN(down); TC_RUN(neg); TC_RUN(zero_exp); TC_RUN(big); return tc_end(); }
