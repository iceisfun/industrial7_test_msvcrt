/* TC
 * id:    pfi_int_flag_hash_o_prec0_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.6.1; MS docs printf type field characters / flag directives
 * note:  PROBE: %#.0o of 0 yields "0" (C11 7.21.6.1p6 says the alternate form forces a leading 0) and %.0o of 0 yields "" on real Windows.
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

#define TU(fmt, v, want) do { char b[64]; volatile unsigned tc_v = (v); int n = sprintf(b, fmt, tc_v); CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want)-1)); } while (0)
TC_CASE(hash_prec0) { TU("%#.0o", 0u, "0"); }
TC_CASE(nohash_prec0) { TU("%.0o", 0u, ""); }

int main(void) { tc_begin(); TC_RUN(hash_prec0); TC_RUN(nohash_prec0); return tc_end(); }
