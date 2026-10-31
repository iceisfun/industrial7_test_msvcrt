/* TC
 * id:    pff_f_precision_zero
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 (precision 0 and # flag); MS docs printf flag directives
 * note:  non-tie inputs only
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

/* sprintf one double with fmt, compare string and return value (== strlen of expected) */
#define TF(fmt, val, want) do { char b[1100]; volatile double tc_v = (val); int n = sprintf(b, fmt, tc_v); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

TC_CASE(round) { TF("%.0f", 2.7, "3"); TF("%.0f", 0.3, "0"); TF("%.0f", 7.0, "7"); }
TC_CASE(neg_small) { TF("%.0f", -0.3, "-0"); }
TC_CASE(hash) { TF("%#.0f", 3.0, "3."); TF("%#.0f", 0.0, "0."); }
TC_CASE(big_exact) { TF("%.0f", 1e15, "1000000000000000"); }

int main(void) { tc_begin(); TC_RUN(round); TC_RUN(neg_small); TC_RUN(hash); TC_RUN(big_exact); return tc_end(); }
