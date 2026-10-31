/* TC
 * id:    mth_neg_pow_negbase_overflow_sign
 * area:  math_basic
 * funcs: pow
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.1, F.10.4.4; MS docs pow
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

TC_CASE(odd_overflow) { double r; errno = 0; r = pow(D(-10.0), D(309.0)); CHECK_BITS_D(r, -INF()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(even_overflow) { double r; errno = 0; r = pow(D(-10.0), D(310.0)); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(even_fits) { double r; errno = 0; r = pow(D(-10.0), D(308.0)); CHECK_NEAR(r / 1e308, 1.0, 1e-12); CHECK_EQ_I(errno, 0); }
TC_CASE(dbl_max_sq) { double r; errno = 0; r = pow(D(-DBL_MAX), D(2.0)); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); TC_RUN(odd_overflow); TC_RUN(even_overflow); TC_RUN(even_fits); TC_RUN(dbl_max_sq); return tc_end(); }
