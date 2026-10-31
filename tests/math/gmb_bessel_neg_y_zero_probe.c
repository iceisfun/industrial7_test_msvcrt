/* TC
 * id:    gmb_bessel_neg_y_zero_probe
 * area:  math
 * funcs: _y0 _y1 _yn
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   Y_n(0) is a pole
 * note:  PROBE: _y0/_y1/_yn at +0 return -inf (value only, errno unasserted beyond being one of 0/EDOM/ERANGE); and _y0(-0.0) value (sign of zero may be treated as negative -> NaN or -inf; recorded as one of NaN or -inf).
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static int neg_inf(double d) { return tc_dbits(d) == 0xFFF0000000000000ULL; }
static int errno_ok(int e) { return e == 0 || e == EDOM || e == ERANGE; }

TC_CASE(y0_zero) { volatile double x = 0.0; double r; errno = 0; r = _y0(x); CHECK(neg_inf(r)); CHECK(errno_ok(errno)); }
TC_CASE(y1_zero) { volatile double x = 0.0; double r; errno = 0; r = _y1(x); CHECK(neg_inf(r)); CHECK(errno_ok(errno)); }
TC_CASE(yn_zero) { volatile double x = 0.0; double r; errno = 0; r = _yn(3, x); CHECK(neg_inf(r)); r = _yn(2, x); CHECK(neg_inf(r)); }
TC_CASE(y0_negzero) { volatile double x = -0.0; double r = _y0(x); CHECK(neg_inf(r) || r != r); }
int main(void) { tc_begin(); TC_RUN(y0_zero); TC_RUN(y1_zero); TC_RUN(yn_zero); TC_RUN(y0_negzero); return tc_end(); }
