/* TC
 * id:    mth_neg_boundary_no_domain_error
 * area:  math_basic
 * funcs: asin acos log sqrt log10 fmod
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.1, F.10.3, F.10.4.3, F.10.7.1; MS docs asin, acos, log, sqrt
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

TC_CASE(asin_pm1) { double r; errno = 0; r = asin(D(1.0)); CHECK_NEAR(r, PI / 2, 1e-15); CHECK_EQ_I(errno, 0); r = asin(D(-1.0)); CHECK_NEAR(r, -PI / 2, 1e-15); CHECK_EQ_I(errno, 0); }
TC_CASE(acos_pm1) { double r; errno = 0; r = acos(D(1.0)); CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, 0); r = acos(D(-1.0)); CHECK_NEAR(r, PI, 1e-15); CHECK_EQ_I(errno, 0); }
TC_CASE(log_one) { double r; errno = 0; r = log(D(1.0)); CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, 0); r = log10(D(1.0)); CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, 0); }
TC_CASE(sqrt_zeros) { double r; errno = 0; r = sqrt(D(-0.0)); CHECK_BITS_D(r, -0.0); CHECK_EQ_I(errno, 0); r = sqrt(D(0.0)); CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, 0); }
TC_CASE(fmod_zeros) { double r; errno = 0; r = fmod(D(0.0), D(1.0)); CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, 0); r = fmod(D(-0.0), D(1.0)); CHECK_BITS_D(r, -0.0); CHECK_EQ_I(errno, 0); }
TC_CASE(log_denormal) { double r; errno = 0; r = log(D(4.9406564584124654e-324)); CHECK_NEAR(r, -744.4400719213812, 1e-6); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_RUN(asin_pm1); TC_RUN(acos_pm1); TC_RUN(log_one); TC_RUN(sqrt_zeros); TC_RUN(fmod_zeros); TC_RUN(log_denormal); return tc_end(); }
