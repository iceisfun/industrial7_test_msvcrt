/* TC
 * id:    pff_f_17digits_legacy_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win7
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf (legacy CRT)
 * note:  PROBE: pre-UCRT printf produces at most 17 significant digits, then pads with zeros (0.1 %.20f -> 0.10000000000000001000).
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

/* sprintf one double with fmt, compare string and return value (== strlen of expected) */
#define TF(fmt, val, want) do { char b[1100]; volatile double tc_v = (val); int n = sprintf(b, fmt, tc_v); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

static volatile double v_one = 1.0, v_three = 3.0;
TC_CASE(tenth) { TF("%.20f", 0.1, "0.10000000000000001000"); }
TC_CASE(third) { TF("%.25f", v_one / v_three, "0.3333333333333333100000000"); }
TC_CASE(tiny) { TF("%.20f", 1e-10, "0.00000000010000000000"); }

int main(void) { tc_begin(); TC_RUN(tenth); TC_RUN(third); TC_RUN(tiny); return tc_end(); }
