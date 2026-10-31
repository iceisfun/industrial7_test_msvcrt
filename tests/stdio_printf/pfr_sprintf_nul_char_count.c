/* TC
 * id:    pfr_sprintf_nul_char_count
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.21.6.1 (%c), 7.21.6.6
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(embedded) { char b[16]; volatile int z = 0; int n; b[2] = 'Q'; n = sprintf(b, "a%cb", z); CHECK_EQ_I(n, 3); CHECK_EQ_I(b[0], 'a'); CHECK_EQ_I(b[1], 0); CHECK_EQ_I(b[2], 'b'); CHECK_EQ_I(b[3], 0); }
TC_CASE(only_nul) { char b[16]; volatile int z = 0; int n; b[1] = 'Q'; n = sprintf(b, "%c", z); CHECK_EQ_I(n, 1); CHECK_EQ_I(b[0], 0); }

int main(void) { tc_begin(); TC_RUN(embedded); TC_RUN(only_nul); return tc_end(); }
