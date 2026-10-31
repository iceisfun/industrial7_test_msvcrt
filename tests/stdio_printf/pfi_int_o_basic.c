/* TC
 * id:    pfi_int_o_basic
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
TC_CASE(small) { TU("%o", 8u, "10"); TU("%o", 0u, "0"); TU("%o", 511u, "777"); TU("%o", 7u, "7"); TU("%o", 64u, "100"); }
TC_CASE(max) { TU("%o", 4294967295u, "37777777777"); TU("%o", 2147483648u, "20000000000"); }
TC_CASE(minus_one) { volatile int i = -1; char b[32]; sprintf(b, "%o", i); CHECK_STR(b, "37777777777"); }
TC_CASE(width_prec) { TU("[%6o]", 8u, "[    10]"); TU("[%-6o]", 8u, "[10    ]"); TU("[%.4o]", 8u, "[0010]"); TU("[%06o]", 8u, "[000010]"); TU("[%.0o]", 0u, "[]"); }

int main(void) { tc_begin(); TC_RUN(small); TC_RUN(max); TC_RUN(minus_one); TC_RUN(width_prec); return tc_end(); }
