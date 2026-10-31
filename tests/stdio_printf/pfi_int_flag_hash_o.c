/* TC
 * id:    pfi_int_flag_hash_o
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1; MS docs printf type field characters / flag directives
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

#define TU(fmt, v, want) do { char b[64]; volatile unsigned tc_v = (v); int n = sprintf(b, fmt, tc_v); CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want)-1)); } while (0)
TC_CASE(basic) { TU("%#o", 8u, "010"); TU("%#o", 0u, "0"); TU("%#o", 511u, "0777"); }
TC_CASE(prec) { TU("%#.5o", 8u, "00010"); TU("%#.2o", 8u, "010"); TU("%#.3o", 8u, "010"); }
TC_CASE(width) { TU("[%#8o]", 8u, "[     010]"); TU("[%#-8o]", 8u, "[010     ]"); TU("[%#08o]", 8u, "[00000010]"); }

int main(void) { tc_begin(); TC_RUN(basic); TC_RUN(prec); TC_RUN(width); return tc_end(); }
