/* TC
 * id:    mth_frexp_signed_denorm_probe
 * area:  math_basic
 * funcs:
 * procs: frexp
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.12.6.4, F.10.3.4
 * note:  PROBE: frexp(-0.0) returns -0.0 with *exp==0, and frexp(denormal) normalises correctly
 *        (2^-1074 -> 0.5, exp -1073) on every CRT incl. legacy ones.
 */
#include "tc.h"
#include <math.h>
TC_DECL_PROC(double, frexp, (double, int *));
#define frexp tc_p_frexp

TC_CASE(neg_zero) { int e = 77; volatile double x = -0.0; CHECK_BITS_D(frexp(x, &e), -0.0); CHECK_EQ_I(e, 0); }
TC_CASE(denorm_min) { int e = 0; union { unsigned long long u; double d; } v; v.u = 1; { volatile double x = v.d; CHECK_BITS_D(frexp(x, &e), 0.5); } CHECK_EQ_I(e, -1073); }

int main(void) { tc_begin(); TC_LOAD_PROC(frexp); if (!tc_p_frexp) { tc_write("TC FAIL " TC_TEST_ID "::frexp not exported\n"); return 1; } TC_RUN(neg_zero); TC_RUN(denorm_min); return tc_end(); }
