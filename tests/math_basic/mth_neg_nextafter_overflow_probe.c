/* TC
 * id:    mth_neg_nextafter_overflow_probe
 * area:  math_basic
 * funcs: _nextafter
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _nextafter; C11 7.12.11.3 (overflow gives +-HUGE_VAL)
 * note:  Probe: _nextafter(DBL_MAX,+inf)=+inf, errno printed (ERANGE or 0); NaN operands propagate.
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

TC_CASE(overflow) { double r = _nextafter(D(DBL_MAX), D(INF())); CHECK_BITS_D(r, INF()); r = _nextafter(D(-DBL_MAX), -D(INF())); CHECK_BITS_D(r, -INF()); }
TC_CASE(errno_info) { errno = 0; _nextafter(D(DBL_MAX), D(INF())); info("nextafter overflow errno", errno); }
TC_CASE(nan) { CHECK_IS_NAN(_nextafter(D(NAN_()), D(1.0))); CHECK_IS_NAN(_nextafter(D(1.0), D(NAN_()))); }
TC_CASE(from_inf) { double r = _nextafter(D(INF()), D(0.0)); CHECK_BITS_D(r, DBL_MAX); }

int main(void) { tc_begin(); TC_RUN(overflow); TC_RUN(errno_info); TC_RUN(nan); TC_RUN(from_inf); return tc_end(); }
