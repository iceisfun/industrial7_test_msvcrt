/* TC
 * id:    pfm_3_prec_hash_o_zero_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.6.1p6 ('#' with o: precision is increased so first digit is 0; so "%#.0o" of 0 gives "0")
 * note:  PROBE: whether every CRT (notably msvcrt.dll / msvcr80) follows C99 for "%#.0o" and "%#.o" with value 0, and "%#.0x" with 0 (expected empty,
 *        since '#' adds 0x only for nonzero). Standard-conforming answer is asserted.
 */
#include "tc.h"
#include <stdio.h>

static volatile unsigned u0 = 0;

TC_CASE(hash_o_prec0)  { char b[32]; sprintf(b, "[%#.0o]", u0); CHECK_STR(b, "[0]"); }
TC_CASE(hash_o_bare)   { char b[32]; sprintf(b, "[%#.o]", u0); CHECK_STR(b, "[0]"); }
TC_CASE(hash_x_prec0)  { char b[32]; sprintf(b, "[%#.0x]", u0); CHECK_STR(b, "[]"); }

int main(void) { tc_begin(); TC_RUN(hash_o_prec0); TC_RUN(hash_o_bare); TC_RUN(hash_x_prec0); return tc_end(); }
