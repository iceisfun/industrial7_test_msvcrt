/* TC
 * id:    mth_neg_pow_zero_negexp_errno_probe
 * area:  math_basic
 * funcs: pow
 * crt:   msvcrt msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.1 (pole error -> ERANGE); older MS docs say EDOM
 * note:  probing errno after pow(+-0, y<0): ERANGE asserted on ucrtbase, EDOM or ERANGE accepted on older CRTs. Value is not asserted here.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double INF(void) { return v_one / v_zero; }
static double D(double x) { volatile double v = x; return v; }

static void chk(double x, double y)
{
    volatile double r; errno = 0; r = pow(D(x), D(y)); (void)r;
#ifdef TC_UCRT
    CHECK_EQ_I(errno, ERANGE);
#else
    CHECK(errno == EDOM || errno == ERANGE);
#endif
}
TC_CASE(pos_zero) { chk(0.0, -1.0); }
TC_CASE(neg_zero) { chk(-0.0, -3.0); }

int main(void) { tc_begin(); TC_RUN(pos_zero); TC_RUN(neg_zero); return tc_end(); }
