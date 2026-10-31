/* TC
 * id:    mth_neg_logb_zero_probe
 * area:  math_basic
 * funcs: _logb
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _logb
 * note:  Probe: _logb(+-0) == -infinity (C logb pole), errno ERANGE or EDOM (unknown which).
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double pinf(void) { return v_one / v_zero; }
static void note_errno(const char *what, int e) { char b[24]; tc_i64(e, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write(" errno="); tc_write(b); tc_write("\n"); }

TC_CASE(pos_zero) { volatile double z = 0.0; CHECK_BITS_D(_logb(z), -pinf()); }
TC_CASE(neg_zero) { volatile double z = -0.0; CHECK_BITS_D(_logb(z), -pinf()); }
TC_CASE(errno_kind) { volatile double z = 0.0; double r; errno = 0; r = _logb(z); (void)r; note_errno("_logb(0)", errno); CHECK(errno == ERANGE || errno == EDOM); }

int main(void) { tc_begin(); TC_RUN(pos_zero); TC_RUN(neg_zero); TC_RUN(errno_kind); return tc_end(); }
