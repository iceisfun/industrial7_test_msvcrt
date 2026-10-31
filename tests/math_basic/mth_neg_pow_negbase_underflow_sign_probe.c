/* TC
 * id:    mth_neg_pow_negbase_underflow_sign_probe
 * area:  math_basic
 * funcs: pow
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 F.10.4.4, 7.12.1 (underflow result is a signed zero)
 * note:  Probe: sign of the zero on pow underflow: pow(10,-400)=+0, pow(-10,-401)=-0 (odd integer exponent), pow(-10,-400)=+0.
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

TC_CASE(pos) { double r = pow(D(10.0), D(-400.0)); CHECK_BITS_D(r, 0.0); }
TC_CASE(neg_odd) { double r; errno = 0; r = pow(D(-10.0), D(-401.0)); CHECK_BITS_D(r, -0.0); info("pow(-10,-401) errno", errno); }
TC_CASE(neg_even) { double r = pow(D(-10.0), D(-400.0)); CHECK_BITS_D(r, 0.0); }

int main(void) { tc_begin(); TC_RUN(pos); TC_RUN(neg_odd); TC_RUN(neg_even); return tc_end(); }
