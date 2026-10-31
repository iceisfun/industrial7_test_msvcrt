/* TC
 * id:    pfi_int_u_basic
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
TC_CASE(basic) { TU("%u", 0u, "0"); TU("%u", 1u, "1"); TU("%u", 4294967295u, "4294967295"); TU("%u", 2147483648u, "2147483648"); }
TC_CASE(minus_one_unsigned) { volatile unsigned u = (unsigned)-1; char b[32]; sprintf(b, "%u", u); CHECK_STR(b, "4294967295"); }
TC_CASE(minus_one_int) { volatile int i = -1; char b[32]; sprintf(b, "%u", i); CHECK_STR(b, "4294967295"); }
TC_CASE(width_prec) { TU("[%6u]", 42u, "[    42]"); TU("[%-6u]", 42u, "[42    ]"); TU("[%.5u]", 42u, "[00042]"); TU("[%8.5u]", 42u, "[   00042]");
  TU("[%06u]", 42u, "[000042]"); TU("[%.0u]", 0u, "[]"); }
TC_CASE(no_sign_flags) { TU("[%+u]", 5u, "[5]"); TU("[% u]", 5u, "[5]"); }

int main(void) { tc_begin(); TC_RUN(basic); TC_RUN(minus_one_unsigned); TC_RUN(minus_one_int); TC_RUN(width_prec); TC_RUN(no_sign_flags); return tc_end(); }
