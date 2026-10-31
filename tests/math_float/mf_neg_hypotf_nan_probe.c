/* TC
 * id:    mf_neg_hypotf_nan_probe
 * area:  math_float
 * funcs: _hypotf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C99 F.10.4.3 hypot
 * note:  PROBE: (1) _hypotf(NaN,1) is NaN; (2) _hypotf(inf,NaN) is +inf per C99 (older MS CRTs may return NaN).
 */
#include "tc.h"
#include <math.h>
static volatile float one = 1.0f, v_inf, v_nan;
static void init(void) { union { unsigned int u; float f; } x; x.u = 0x7F800000u; v_inf = x.f; x.u = 0x7FC00000u; v_nan = x.f; }

TC_CASE(nan_one) { float r = _hypotf(v_nan, one); CHECK(r != r); }
TC_CASE(inf_nan_is_inf) { CHECK_BITS_F(_hypotf(v_inf, v_nan), v_inf); CHECK_BITS_F(_hypotf(v_nan, -v_inf), v_inf); }
int main(void) { tc_begin(); init(); TC_RUN(nan_one); TC_RUN(inf_nan_is_inf); return tc_end(); }
