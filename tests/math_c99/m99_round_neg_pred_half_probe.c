/* TC
 * id:    m99_round_neg_pred_half_probe
 * area:  math_c99
 * funcs: round roundf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.9.6 (round half away; 0.49999999999999994 is below one half)
 * note:  PROBE: round(0.49999999999999994) must be 0 (naive floor(x+0.5) gives 1). Some early MSVC round
 *        implementations are suspected of this bug; the standard requires 0. Also roundf(0.49999997f).
 */
#include "tc.h"
#include <math.h>

TC_CASE(pred_half_d) { volatile double a = 0.49999999999999994; CHECK_BITS_D(round(a), 0.0); }
TC_CASE(pred_half_f) { volatile float a = 0.49999997f; CHECK_BITS_F(roundf(a), 0.0f); }

int main(void) { tc_begin(); TC_RUN(pred_half_d); TC_RUN(pred_half_f); return tc_end(); }
