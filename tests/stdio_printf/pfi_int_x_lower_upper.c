/* TC
 * id:    pfi_int_x_lower_upper
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
TC_CASE(deadbeef) { TU("%x", 0xdeadbeefu, "deadbeef"); TU("%X", 0xdeadbeefu, "DEADBEEF"); }
TC_CASE(zero) { TU("%x", 0u, "0"); TU("%X", 0u, "0"); }
TC_CASE(minus_one) { volatile int i = -1; char b[32]; sprintf(b, "%x", i); CHECK_STR(b, "ffffffff"); sprintf(b, "%X", i); CHECK_STR(b, "FFFFFFFF"); }
TC_CASE(mixed) { TU("%x", 0x1a2b3cu, "1a2b3c"); TU("%X", 0x1a2b3cu, "1A2B3C"); TU("%x", 0xabcdefu, "abcdef"); TU("%X", 0xabcdefu, "ABCDEF"); TU("%x", 0x10u, "10"); }
TC_CASE(width_prec) { TU("[%6x]", 0xffu, "[    ff]"); TU("[%06X]", 0xffu, "[0000FF]"); TU("[%.4x]", 0xffu, "[00ff]"); TU("[%.0x]", 0u, "[]"); }

int main(void) { tc_begin(); TC_RUN(deadbeef); TC_RUN(zero); TC_RUN(minus_one); TC_RUN(mixed); TC_RUN(width_prec); return tc_end(); }
