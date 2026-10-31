/* TC
 * id:    mth_neg_asin_acos_inf_domain
 * area:  math_basic
 * funcs: asin acos
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.1.1, F.10.1.2 (|x|>1 is a domain error), 7.12.1; MS docs asin, acos
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

TC_CASE(asin_inf) { double r; errno = 0; r = asin(D(INF())); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); errno = 0; r = asin(-D(INF())); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }
TC_CASE(acos_inf) { double r; errno = 0; r = acos(D(INF())); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); errno = 0; r = acos(-D(INF())); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }

int main(void) { tc_begin(); TC_RUN(asin_inf); TC_RUN(acos_inf); return tc_end(); }
