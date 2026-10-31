/* TC
 * id:    mth_neg_tanh_atan_never_error
 * area:  math_basic
 * funcs: tanh atan atan2
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.1, F.10.1.4, F.10.4.4; MS docs tanh, atan
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

TC_CASE(tanh_huge) { double r; errno = 0; r = tanh(D(1000.0)); CHECK_BITS_D(r, 1.0); CHECK_EQ_I(errno, 0); r = tanh(D(-1000.0)); CHECK_BITS_D(r, -1.0); CHECK_EQ_I(errno, 0);
    r = tanh(D(DBL_MAX)); CHECK_BITS_D(r, 1.0); CHECK_EQ_I(errno, 0); r = tanh(D(-DBL_MAX)); CHECK_BITS_D(r, -1.0); CHECK_EQ_I(errno, 0); }
TC_CASE(atan_huge) { double r; errno = 0; r = atan(D(DBL_MAX)); CHECK_NEAR(r, PI / 2, 1e-15); CHECK_EQ_I(errno, 0); r = atan(D(-DBL_MAX)); CHECK_NEAR(r, -PI / 2, 1e-15); CHECK_EQ_I(errno, 0); }
TC_CASE(atan2_extremes) { double r; errno = 0;
    r = atan2(D(DBL_MIN), D(-DBL_MAX)); CHECK_NEAR(r, PI, 1e-15); CHECK(r == r && r <= 3.1415926535897936); CHECK_EQ_I(errno, 0);
    r = atan2(D(-DBL_MIN), D(-DBL_MAX)); CHECK_NEAR(r, -PI, 1e-15); CHECK_EQ_I(errno, 0);
    r = atan2(D(DBL_MAX), D(DBL_MIN)); CHECK_NEAR(r, PI / 2, 1e-15); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_RUN(tanh_huge); TC_RUN(atan_huge); TC_RUN(atan2_extremes); return tc_end(); }
