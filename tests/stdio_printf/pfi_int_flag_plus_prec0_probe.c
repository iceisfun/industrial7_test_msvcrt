/* TC
 * id:    pfi_int_flag_plus_prec0_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.1p4,p8 (zero value with precision 0 yields no characters; + forces a sign)
 * note:  PROBE: whether "%+.0d" of 0 yields "+" (sign emitted, no digits) or "" on real Windows. The standard is ambiguous about the sign here; "+" is the usual implementation result.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(plus_prec0_zero) { char b[64]; volatile int v = 0; int n = sprintf(b, "[%+.0d]", v); CHECK_STR(b, "[+]"); CHECK_EQ_I(n, 3); }

int main(void) { tc_begin(); TC_RUN(plus_prec0_zero); return tc_end(); }
