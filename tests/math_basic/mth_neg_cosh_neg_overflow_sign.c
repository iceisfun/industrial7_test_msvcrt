/* TC
 * id:    mth_neg_cosh_neg_overflow_sign
 * area:  math_basic
 * funcs: cosh sinh exp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.1, 7.12.5.4-5, 7.12.6.1; MS docs exp, sinh, cosh
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

TC_CASE(exp_threshold) { double r; errno = 0; r = exp(D(709.78)); CHECK(r > 1e308 && r < INF()); CHECK_EQ_I(errno, 0); errno = 0; r = exp(D(709.79)); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(sinh_over) { double r; errno = 0; r = sinh(D(709.0)); CHECK(r > 1e307 && r < INF()); CHECK_EQ_I(errno, 0);
    errno = 0; r = sinh(D(710.5)); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, ERANGE); errno = 0; r = sinh(D(-710.5)); CHECK_BITS_D(r, -INF()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(cosh_over) { double r; errno = 0; r = cosh(D(710.5)); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, ERANGE); errno = 0; r = cosh(D(-710.5)); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(dbl_max) { double r; errno = 0; r = exp(D(DBL_MAX)); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, ERANGE); errno = 0; r = sinh(D(-DBL_MAX)); CHECK_BITS_D(r, -INF()); CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); TC_RUN(exp_threshold); TC_RUN(sinh_over); TC_RUN(cosh_over); TC_RUN(dbl_max); return tc_end(); }
