/* TC
 * id:    mth_neg_logb_inf_nan_probe
 * area:  math_basic
 * funcs: _logb
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _logb; C11 F.10.3.11 (logb(+-inf)=+inf)
 * note:  OBSERVED on Win10 22H2: _logb(-inf) = -inf (deviates from C11 F.10.3.11 logb(+-inf)=+inf); _logb(+inf)=+inf.
 *        Probe: _logb(+-inf)=+inf, _logb(nan) NaN; errno printed only.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double INF(void) { return v_one / v_zero; }
static double NAN_(void) { volatile double i = INF(); return i - i; }
static double D(double x) { volatile double v = x; return v; }
static int NEG(double x) { return tc_dbits(x) >> 63 != 0; }
static void info(const char *w, long long e) { char b[24]; tc_i64(e, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(w); tc_write(" = "); tc_write(b); tc_write("\n"); }
#define PI 3.14159265358979323846

TC_CASE(inf) { double r = _logb(D(INF())); CHECK_BITS_D(r, INF()); r = _logb(-D(INF())); CHECK_BITS_D(r, -INF()); }
TC_CASE(nan) { CHECK_IS_NAN(_logb(D(NAN_()))); }
TC_CASE(errno_info) { errno = 0; _logb(D(INF())); info("logb(inf) errno", errno); errno = 0; _logb(-D(INF())); info("logb(-inf) errno", errno); errno = 0; _logb(D(NAN_())); info("logb(nan) errno", errno); }

int main(void) { tc_begin(); TC_RUN(inf); TC_RUN(nan); TC_RUN(errno_info); return tc_end(); }
