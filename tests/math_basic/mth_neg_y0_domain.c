/* TC
 * id:    mth_neg_y0_domain
 * area:  math_basic
 * funcs: _y0 _y1
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _j0/_y0 (_DOMAIN error for negative x)
 * note:  Probe: _y0(-1) and _y1(-1) return NaN. Docs name a _DOMAIN error but the exact return value (NaN vs -inf) is unverified.
 */
#include "tc.h"
#include <math.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
TC_CASE(negative_arg) { volatile double x=-1.0; CHECK_IS_NAN(_y0(x)); CHECK_IS_NAN(_y1(x)); }

int main(void) { tc_begin(); TC_RUN(negative_arg); return tc_end(); }
