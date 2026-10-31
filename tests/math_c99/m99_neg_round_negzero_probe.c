/* TC
 * id:    m99_neg_round_negzero_probe
 * area:  math_c99
 * funcs: trunc round rint nearbyint
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   IEEE 754 roundToIntegral (sign of the operand is preserved, so a negative input rounding to zero gives -0)
 * note:  PROBE: nonzero negative inputs that round to zero yield -0.0 (sign bit set), not +0.0, for trunc/round/rint/nearbyint.
 */
#include "tc.h"
#include <math.h>
TC_CASE(trunc_neg) { volatile double a = -0.9; CHECK_BITS_D(trunc(a), -0.0); }
TC_CASE(round_neg) { volatile double b = -0.4; CHECK_BITS_D(round(b), -0.0); }
TC_CASE(rint_neg) { volatile double c = -0.3, e = -0.5; CHECK_BITS_D(rint(c), -0.0); CHECK_BITS_D(rint(e), -0.0); }
TC_CASE(nearbyint_neg) { volatile double c = -0.3, e = -0.5; CHECK_BITS_D(nearbyint(c), -0.0); CHECK_BITS_D(nearbyint(e), -0.0); }
int main(void) { tc_begin(); TC_RUN(trunc_neg); TC_RUN(round_neg); TC_RUN(rint_neg); TC_RUN(nearbyint_neg); return tc_end(); }
