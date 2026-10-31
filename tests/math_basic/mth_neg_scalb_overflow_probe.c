/* TC
 * id:    mth_neg_scalb_overflow_probe
 * area:  math_basic
 * funcs: _scalb
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _scalb
 * note:  Probe: _scalb(1,5000)=+inf, _scalb(1,-5000)=+0, errno ERANGE on both (errno is a guess).
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double pinf(void) { return v_one / v_zero; }
static void note_errno(const char *what, int e) { char b[24]; tc_i64(e, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write(" errno="); tc_write(b); tc_write("\n"); }

TC_CASE(overflow) { volatile double x = 1.0; volatile long e = 5000; CHECK_BITS_D(_scalb(x, e), pinf()); }
TC_CASE(underflow) { volatile double x = 1.0; volatile long e = -5000; CHECK_BITS_D(_scalb(x, e), 0.0); }
TC_CASE(errno_erange) { volatile double x = 1.0; volatile long e = 5000; double r; errno = 0; r = _scalb(x, e); (void)r; note_errno("_scalb(1,5000)", errno); CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); TC_RUN(overflow); TC_RUN(underflow); TC_RUN(errno_erange); return tc_end(); }
