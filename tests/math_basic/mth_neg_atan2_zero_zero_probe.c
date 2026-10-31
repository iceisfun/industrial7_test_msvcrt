/* TC
 * id:    mth_neg_atan2_zero_zero_probe
 * area:  math_basic
 * funcs: atan2
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs atan2
 * note:  Probe: atan2(0,0) returns +0 (documented); errno is EDOM on pre-UCRT per old docs, unknown on ucrt (only recorded there).
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double pinf(void) { return v_one / v_zero; }
static void note_errno(const char *what, int e) { char b[24]; tc_i64(e, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write(" errno="); tc_write(b); tc_write("\n"); }

TC_CASE(value) { volatile double y = 0.0, x = 0.0; CHECK_BITS_D(atan2(y, x), 0.0); }
TC_CASE(errno_recorded) { volatile double y = 0.0, x = 0.0; double r; errno = 0; r = atan2(y, x); (void)r; note_errno("atan2(0,0)", errno);
#ifndef TC_UCRT
    CHECK_EQ_I(errno, EDOM);
#endif
}

int main(void) { tc_begin(); TC_RUN(value); TC_RUN(errno_recorded); return tc_end(); }
