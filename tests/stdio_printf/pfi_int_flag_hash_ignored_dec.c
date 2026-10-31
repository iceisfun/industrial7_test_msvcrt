/* TC
 * id:    pfi_int_flag_hash_ignored_dec
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.6.1p6 (undefined for d i u); MS docs printf flag directives
 * note:  PROBE: whether the # flag is silently ignored for %d %i %u on real Windows (C leaves it undefined; MS docs list # only for o x X c e f g a).
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

#define T(fmt, v, want) do { char b[64]; volatile int tc_v = (v); int n = sprintf(b, fmt, tc_v); CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want)-1)); } while (0)

#define TU(fmt, v, want) do { char b[64]; volatile unsigned tc_v = (v); int n = sprintf(b, fmt, tc_v); CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want)-1)); } while (0)
TC_CASE(hash_d) { T("%#d", 5, "5"); T("%#d", -5, "-5"); T("%#d", 0, "0"); }
TC_CASE(hash_i) { T("%#i", 5, "5"); }
TC_CASE(hash_u) { TU("%#u", 5u, "5"); TU("%#u", 0u, "0"); }
TC_CASE(hash_width) { T("[%#5d]", 5, "[    5]"); }

int main(void) { tc_begin(); TC_RUN(hash_d); TC_RUN(hash_i); TC_RUN(hash_u); TC_RUN(hash_width); return tc_end(); }
