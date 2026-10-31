/* TC
 * id:    mth_neg_nextafter_underflow_probe
 * area:  math_basic
 * funcs: _nextafter
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _nextafter; C11 7.12.11.3
 * note:  Probe: _nextafter stepping into denormals and to zero, zero signs, x==y case, errno printed only.
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

TC_CASE(largest_denormal) { double r = _nextafter(D(DBL_MIN), D(0.0)); CHECK_BITS_D(r, 2.2250738585072009e-308); CHECK_EQ_U(tc_dbits(r), 0x000fffffffffffffULL); errno = 0; _nextafter(D(DBL_MIN), D(0.0)); info("nextafter into denormal errno", errno); }
TC_CASE(to_zero) { double r = _nextafter(D(4.9406564584124654e-324), D(0.0)); CHECK_BITS_D(r, 0.0); r = _nextafter(D(-4.9406564584124654e-324), D(0.0)); CHECK_BITS_D(r, -0.0); }
TC_CASE(zero_negzero) { double r = _nextafter(D(0.0), D(-0.0)); info("nextafter(0,-0) signbit", NEG(r)); CHECK_BITS_D(r, -0.0); }
TC_CASE(equal) { double r = _nextafter(D(1.5), D(1.5)); CHECK_BITS_D(r, 1.5); r = _nextafter(D(-0.0), D(0.0)); CHECK_BITS_D(r, 0.0); }

int main(void) { tc_begin(); TC_RUN(largest_denormal); TC_RUN(to_zero); TC_RUN(zero_negzero); TC_RUN(equal); return tc_end(); }
