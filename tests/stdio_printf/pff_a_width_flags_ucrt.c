/* TC
 * id:    pff_a_width_flags_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 a/A flags and width
 */
#include "tc.h"
#include <stdio.h>

static volatile double one = 1.0, onehalf = 1.5;

TC_CASE(right) { char b[64]; sprintf(b, "[%20.3a]", one); CHECK_STR(b, "[          0x1.000p+0]"); }
TC_CASE(left) { char b[64]; sprintf(b, "[%-20.3a]", one); CHECK_STR(b, "[0x1.000p+0          ]"); }
TC_CASE(plus) { char b[64]; sprintf(b, "[%+.1a]", onehalf); CHECK_STR(b, "[+0x1.8p+0]"); }
TC_CASE(space) { char b[64]; sprintf(b, "[% .1a]", onehalf); CHECK_STR(b, "[ 0x1.8p+0]"); }
TC_CASE(zero_after_prefix) { char b[64]; sprintf(b, "[%020.3a]", one); CHECK_STR(b, "[0x00000000001.000p+0]"); }

int main(void) { tc_begin(); TC_RUN(right); TC_RUN(left); TC_RUN(plus); TC_RUN(space); TC_RUN(zero_after_prefix); return tc_end(); }
