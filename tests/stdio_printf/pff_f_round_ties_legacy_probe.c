/* TC
 * id:    pff_f_round_ties_legacy_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win7
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf (legacy CRT)
 * note:  PROBE: pre-UCRT CRTs round the generated decimal digits half away from zero, so exact ties do not go to even (0.5->1, 2.5->3).
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

/* sprintf one double with fmt, compare string and return value (== strlen of expected) */
#define TF(fmt, val, want) do { char b[1100]; volatile double tc_v = (val); int n = sprintf(b, fmt, tc_v); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

TC_CASE(prec0) { TF("%.0f", 0.5, "1"); TF("%.0f", 1.5, "2"); TF("%.0f", 2.5, "3"); }
TC_CASE(prec2) { TF("%.2f", 0.125, "0.13"); TF("%.2f", 0.375, "0.38"); }

int main(void) { tc_begin(); TC_RUN(prec0); TC_RUN(prec2); return tc_end(); }
