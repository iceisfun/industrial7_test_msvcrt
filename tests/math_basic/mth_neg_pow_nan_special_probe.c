/* TC
 * id:    mth_neg_pow_nan_special_probe
 * area:  math_basic
 * funcs: pow
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 F.10.4.4 (pow(x,+-0)=1, pow(1,y)=1 even for NaN)
 * note:  Probe: pow(NaN,0)==1 and pow(1,NaN)==1 with errno 0 on ucrtbase (older MS CRTs returned NaN for pow(1,NaN)).
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

TC_CASE(nan_zero) { double r; errno = 0; r = pow(D(NAN_()), D(0.0)); CHECK_BITS_D(r, 1.0); CHECK_EQ_I(errno, 0); }
TC_CASE(one_nan) { double r; errno = 0; r = pow(D(1.0), D(NAN_())); CHECK_BITS_D(r, 1.0); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_RUN(nan_zero); TC_RUN(one_nan); return tc_end(); }
