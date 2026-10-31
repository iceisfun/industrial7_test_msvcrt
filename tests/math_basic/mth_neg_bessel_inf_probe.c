/* TC
 * id:    mth_neg_bessel_inf_probe
 * area:  math_basic
 * funcs: _j1 _jn _y0 _y1 _yn
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _j0, _j1, _jn, _y0, _y1, _yn (infinity behaviour undocumented)
 * note:  PROBE: Bessel functions at +inf. Mathematically the limit is 0; some libms return NaN. Assert only: result is 0 or NaN,
 *        never +-inf. Actual value class and errno are printed.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0, v_nan_src;
static double pinf(void) { return v_one / v_zero; }
static double qnan(void) { return v_zero / v_zero; }
static int sgn(double d) { return (int)(tc_dbits(d) >> 63); }
static void info_i(const char *what, long long v) { char b[32]; tc_i64(v, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write("="); tc_write(b); tc_write("\n"); }

static void chk(const char *what, double r)
{
    info_i(what, r == 0.0 ? 0 : (r != r ? 1 : 2));
    CHECK(r == 0.0 || r != r);
}
TC_CASE(j1) { volatile double x = pinf(); double r; errno = 0; r = _j1(x); chk("j1_0zero_1nan", r); info_i("j1_errno", errno); }
TC_CASE(jn) { volatile double x = pinf(); double r; errno = 0; r = _jn(2, x); chk("jn_0zero_1nan", r); info_i("jn_errno", errno); }
TC_CASE(y0) { volatile double x = pinf(); double r; errno = 0; r = _y0(x); chk("y0_0zero_1nan", r); info_i("y0_errno", errno); }
TC_CASE(y1) { volatile double x = pinf(); double r; errno = 0; r = _y1(x); chk("y1_0zero_1nan", r); info_i("y1_errno", errno); }
TC_CASE(yn) { volatile double x = pinf(); double r; errno = 0; r = _yn(3, x); chk("yn_0zero_1nan", r); info_i("yn_errno", errno); }

int main(void) { tc_begin(); TC_RUN(j1); TC_RUN(jn); TC_RUN(y0); TC_RUN(y1); TC_RUN(yn); return tc_end(); }
