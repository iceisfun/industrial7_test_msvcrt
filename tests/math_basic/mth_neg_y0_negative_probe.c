/* TC
 * id:    mth_neg_y0_negative_probe
 * area:  math_basic
 * funcs: _y0
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _j0/_j1/_jn/_y0/_y1/_yn (_DOMAIN error for negative x)
 * note:  Probe: _y0 of a negative argument is not finite (nan or -inf) and errno==EDOM.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double pinf(void) { return v_one / v_zero; }
static void note_errno(const char *what, int e) { char b[24]; tc_i64(e, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write(" errno="); tc_write(b); tc_write("\n"); }

TC_CASE(not_finite) { volatile double x = -1.0; double r = _y0(x); CHECK(r != r || r == pinf() || r == -pinf()); }
TC_CASE(errno_edom) { volatile double x = -1.0; double r; errno = 0; r = _y0(x); (void)r; note_errno("_y0(-1)", errno); CHECK_EQ_I(errno, EDOM); }

int main(void) { tc_begin(); TC_RUN(not_finite); TC_RUN(errno_edom); return tc_end(); }
