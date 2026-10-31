/* TC
 * id:    pff_f_flags_combo
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1; MS docs printf type field characters f; flag directives
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

/* sprintf one double with fmt, compare string and return value (== strlen of expected) */
#define TF(fmt, val, want) do { char b[1100]; volatile double tc_v = (val); int n = sprintf(b, fmt, tc_v); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

TC_CASE(plus) { TF("[%+f]", 1.5, "[+1.500000]"); TF("[%+f]", -1.5, "[-1.500000]"); }
TC_CASE(space) { TF("[% f]", 1.5, "[ 1.500000]"); TF("[% f]", -1.5, "[-1.500000]"); }
TC_CASE(plus_over_space) { TF("[% +f]", 1.5, "[+1.500000]"); TF("[%+ f]", 1.5, "[+1.500000]"); }
TC_CASE(zero) { TF("[%08.2f]", -3.14159, "[-0003.14]"); TF("[%08.2f]", 3.14159, "[00003.14]"); }
TC_CASE(zero_plus) { TF("[%+08.2f]", 3.14159, "[+0003.14]"); }
TC_CASE(minus_beats_zero) { TF("[%-8.2f]", 3.14159, "[3.14    ]"); TF("[%-08.2f]", 3.14159, "[3.14    ]"); }

int main(void) { tc_begin(); TC_RUN(plus); TC_RUN(space); TC_RUN(plus_over_space); TC_RUN(zero); TC_RUN(zero_plus); TC_RUN(minus_beats_zero); return tc_end(); }
