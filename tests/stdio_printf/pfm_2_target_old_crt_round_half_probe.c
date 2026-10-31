/* TC
 * id:    pfm_2_target_old_crt_round_half_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.1 (rounding is implementation-defined for exactly representable halfway values)
 * note:  PROBES that pre-UCRT CRTs round an exactly representable decimal tie away from zero in %.0f (2.5 -> "3", 0.5 -> "1"),
 *        unlike a round-half-even implementation ("2", "0"). Believed from long-standing msvcr behaviour, not documented.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(ties_away)
{
    char b[64]; volatile double d = 2.5, e = 0.5; sprintf(b, "%.0f|%.0f", d, e);
    CHECK_STR(b, "3|1");
}
int main(void) { tc_begin(); TC_RUN(ties_away); return tc_end(); }
