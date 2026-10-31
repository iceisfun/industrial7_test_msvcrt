/* TC
 * id:    mth_neg_ldexp_underflow_probe
 * area:  math_basic
 * funcs: ldexp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.6.6; MS docs ldexp
 * note:  Probe: ldexp(1,-1100) underflows to +0, ldexp(-1,INT_MIN) to -0 (sign kept); errno ERANGE on underflow is a guess (C allows either).
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double pinf(void) { return v_one / v_zero; }
static void note_errno(const char *what, int e) { char b[24]; tc_i64(e, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write(" errno="); tc_write(b); tc_write("\n"); }

TC_CASE(underflow_pos) { volatile double x = 1.0; volatile int e = -1100; double r = ldexp(x, e); CHECK_BITS_D(r, 0.0); }
TC_CASE(underflow_neg_sign) { volatile double x = -1.0; volatile int e = INT_MIN; double r = ldexp(x, e); CHECK_BITS_D(r, -0.0); }
TC_CASE(errno_erange) { volatile double x = 1.0; volatile int e = -1100; double r; errno = 0; r = ldexp(x, e); (void)r; note_errno("ldexp(1,-1100)", errno); CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); TC_RUN(underflow_pos); TC_RUN(underflow_neg_sign); TC_RUN(errno_erange); return tc_end(); }
