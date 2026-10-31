/* TC
 * id:    mth_neg_pow_negzero_oddneg_probe
 * area:  math_basic
 * funcs: pow
 * crt:   msvcrt msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 F.10.4.4 (pow(+-0, y<0) pole error, odd integer -> +-inf)
 * note:  Probe: pow(-0.0, negative) signs (-inf for odd integer, +inf otherwise); errno printed, not asserted (EDOM or ERANGE).
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

TC_CASE(odd) { double r = pow(D(-0.0), D(-3.0)); CHECK_BITS_D(r, -INF()); }
TC_CASE(even) { double r = pow(D(-0.0), D(-2.0)); CHECK_BITS_D(r, INF()); }
TC_CASE(nonint) { double r = pow(D(-0.0), D(-2.5)); CHECK_BITS_D(r, INF()); }
TC_CASE(errno_info) { double r; errno = 0; r = pow(D(-0.0), D(-1.0)); info("pow(-0,-1) errno", errno); (void)r; }

int main(void) { tc_begin(); TC_RUN(odd); TC_RUN(even); TC_RUN(nonint); TC_RUN(errno_info); return tc_end(); }
