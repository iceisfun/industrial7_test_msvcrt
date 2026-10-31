/* TC
 * id:    mth_frexp_zero_denorm
 * area:  math_basic
 * funcs:
 * procs: frexp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.12.6.4; MS docs frexp (zero: returns 0, *exp = 0)
 * note:  -0.0 and denormal inputs are covered separately in mth_frexp_signed_denorm_probe
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
TC_DECL_PROC(double, frexp, (double, int *));
#define frexp tc_p_frexp
TC_DECL_PROC(double, ldexp, (double, int));
#define ldexp tc_p_ldexp

static volatile double v_zero = 0.0, v_one = 1.0;

TC_CASE(pos_zero) { int e = 77; volatile double x = 0.0; CHECK_BITS_D(frexp(x, &e), 0.0); CHECK_EQ_I(e, 0); }
TC_CASE(dbl_max) { int e = 0; union { unsigned long long u; double d; } v; v.u = 0x7FEFFFFFFFFFFFFFULL; { volatile double x = v.d; double r = frexp(x, &e); CHECK(r < 1.0 && r >= 0.5); CHECK_BITS_D(r, 0.99999999999999988897769753748434595763683319091796875); } CHECK_EQ_I(e, 1024); }

int main(void) { tc_begin(); TC_LOAD_PROC(frexp); TC_LOAD_PROC(ldexp); if (!tc_p_frexp || !tc_p_ldexp) { tc_write("TC FAIL " TC_TEST_ID "::frexp not exported\n"); return 1; } TC_RUN(pos_zero); TC_RUN(dbl_max); return tc_end(); }
