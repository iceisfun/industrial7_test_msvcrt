/* TC
 * id:    mf_neg_hypotf_overflow
 * area:  math_float
 * funcs: _hypotf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _hypot (INFINITY on overflow); C11 F.10.4.3 hypot(+-inf,y) = +inf
 * note:  errno on overflow is NOT asserted for the float variant (see mf_neg_hypotf_overflow_errno_probe).
 */
#include "tc.h"
#include <math.h>
static volatile float big = 3e38f, one = 1.0f, v_inf;
static void init(void) { union { unsigned int u; float f; } x; x.u = 0x7F800000u; v_inf = x.f; }

TC_CASE(overflow_value) { CHECK_BITS_F(_hypotf(big, big), v_inf); }
TC_CASE(inf_inputs) { CHECK_BITS_F(_hypotf(v_inf, one), v_inf); CHECK_BITS_F(_hypotf(-v_inf, one), v_inf); }
int main(void) { tc_begin(); init(); TC_RUN(overflow_value); TC_RUN(inf_inputs); return tc_end(); }
