/* TC
 * id:    pff_f_width_zero_left
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1; MS docs printf type field characters f; width
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

/* sprintf one double with fmt, compare string and return value (== strlen of expected) */
#define TF(fmt, val, want) do { char b[1100]; volatile double tc_v = (val); int n = sprintf(b, fmt, tc_v); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

TC_CASE(right) { TF("[%12f]", 1.5, "[    1.500000]"); }
TC_CASE(left) { TF("[%-12f]", 1.5, "[1.500000    ]"); }
TC_CASE(neg_prec) { TF("[%12.3f]", -1.5, "[      -1.500]"); }
TC_CASE(narrow) { TF("[%3f]", 1.5, "[1.500000]"); }
TC_CASE(zero_neg) { TF("[%012.3f]", -1.5, "[-0000001.500]"); }
TC_CASE(star) { char b[64]; volatile double v = 1.5; sprintf(b, "[%*.*f]", 9, 2, v); CHECK_STR(b, "[     1.50]"); }

int main(void) { tc_begin(); TC_RUN(right); TC_RUN(left); TC_RUN(neg_prec); TC_RUN(narrow); TC_RUN(zero_neg); TC_RUN(star); return tc_end(); }
