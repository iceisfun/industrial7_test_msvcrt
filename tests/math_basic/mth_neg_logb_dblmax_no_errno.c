/* TC
 * id:    mth_neg_logb_dblmax_no_errno
 * area:  math_basic
 * funcs: _logb
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.6.11 logb; MS docs _logb
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

TC_CASE(dbl_max) { double r; errno = 0; r = _logb(D(DBL_MAX)); CHECK_BITS_D(r, 1023.0); CHECK_EQ_I(errno, 0); }
TC_CASE(neg_dbl_max) { double r; errno = 0; r = _logb(D(-DBL_MAX)); CHECK_BITS_D(r, 1023.0); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_RUN(dbl_max); TC_RUN(neg_dbl_max); return tc_end(); }
