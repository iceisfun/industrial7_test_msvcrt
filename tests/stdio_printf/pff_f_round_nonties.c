/* TC
 * id:    pff_f_round_nonties
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 f (correct rounding of the stored binary value)
 * note:  Stored doubles: 2.675=2.67499999999999982.., 1.005=1.00499999999999989.., 0.285=0.28499999999999997.., 0.05=0.05000000000000000277; values chosen so exact rounding and the legacy 17-digit algorithm agree.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

/* sprintf one double with fmt, compare string and return value (== strlen of expected) */
#define TF(fmt, val, want) do { char b[1100]; volatile double tc_v = (val); int n = sprintf(b, fmt, tc_v); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

TC_CASE(below_tie) { TF("%.2f", 2.675, "2.67"); TF("%.2f", 1.005, "1.00"); TF("%.2f", 0.285, "0.28"); }
TC_CASE(above_tie) { TF("%.1f", 0.05, "0.1"); }
TC_CASE(six) { TF("%.6f", 1.0000004, "1.000000"); TF("%.6f", 1.0000006, "1.000001"); }
TC_CASE(neg) { TF("%.2f", -2.675, "-2.67"); TF("%.1f", -0.05, "-0.1"); }

int main(void) { tc_begin(); TC_RUN(below_tie); TC_RUN(above_tie); TC_RUN(six); TC_RUN(neg); return tc_end(); }
