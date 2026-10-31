/* TC
 * id:    pfi_probe_plus_prec0_zero
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.6.1p4,p8 (sign is emitted even when precision 0 and value 0 yield no digits)
 * note:  Probes whether %+.0d / % .0d of 0 yield just the sign ("+" / " ") on every Windows CRT.
 *        MS docs do not describe the zero-value/zero-precision case.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(plus) { char b[16]; volatile int z = 0; int n = sprintf(b, "%+.0d", z); CHECK_STR(b, "+"); CHECK_EQ_I(n, 1); }
TC_CASE(space) { char b[16]; volatile int z = 0; int n = sprintf(b, "% .0d", z); CHECK_STR(b, " "); CHECK_EQ_I(n, 1); }

int main(void) { tc_begin(); TC_RUN(plus); TC_RUN(space); return tc_end(); }
