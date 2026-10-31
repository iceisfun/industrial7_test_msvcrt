/* TC
 * id:    pfi_int_flag_hash_x
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
TC_CASE(lower) { TU("%#x", 255u, "0xff"); TU("%#x", 1u, "0x1"); }
TC_CASE(upper) { TU("%#X", 255u, "0XFF"); TU("%#X", 0xabcu, "0XABC"); }
TC_CASE(zero_no_prefix) { TU("%#x", 0u, "0"); TU("%#X", 0u, "0"); TU("[%#.0x]", 0u, "[]"); }
TC_CASE(width) { TU("[%#8x]", 255u, "[    0xff]"); TU("[%#8X]", 255u, "[    0XFF]"); TU("[%#6x]", 0u, "[     0]"); }
TC_CASE(zero_pad) { TU("%#08x", 255u, "0x0000ff"); TU("%#08X", 255u, "0X0000FF"); }
TC_CASE(minus) { TU("%#-8x|", 255u, "0xff    |"); TU("%#-8X|", 255u, "0XFF    |"); }
TC_CASE(prec) { TU("%#.4x", 255u, "0x00ff"); TU("[%#10.4x]", 255u, "[    0x00ff]"); }

int main(void) { tc_begin(); TC_RUN(lower); TC_RUN(upper); TC_RUN(zero_no_prefix); TC_RUN(width); TC_RUN(zero_pad); TC_RUN(minus); TC_RUN(prec); return tc_end(); }
