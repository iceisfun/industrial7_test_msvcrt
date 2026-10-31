/* TC
 * id:    pff_f_round_ties_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.21.6.1 (IEEE half-even); MS docs printf
 * note:  PROBE: UCRT rounds exactly-representable decimal ties half-to-even (0.5->0, 2.5->2). Older Windows 10 ucrtbase builds rounded half away from zero; the fix shipped in later builds, so this records per-machine behaviour.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

/* sprintf one double with fmt, compare string and return value (== strlen of expected) */
#define TF(fmt, val, want) do { char b[1100]; volatile double tc_v = (val); int n = sprintf(b, fmt, tc_v); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

TC_CASE(prec0) { TF("%.0f", 0.5, "0"); TF("%.0f", 1.5, "2"); TF("%.0f", 2.5, "2"); TF("%.0f", 3.5, "4"); }
TC_CASE(prec2) { TF("%.2f", 0.125, "0.12"); TF("%.2f", 0.375, "0.38"); TF("%.2f", 0.625, "0.62"); }
TC_CASE(neg) { TF("%.0f", -2.5, "-2"); TF("%.0f", -0.5, "-0"); }

int main(void) { tc_begin(); TC_RUN(prec0); TC_RUN(prec2); TC_RUN(neg); return tc_end(); }
