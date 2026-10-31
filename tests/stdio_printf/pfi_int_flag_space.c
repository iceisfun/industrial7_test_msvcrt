/* TC
 * id:    pfi_int_flag_space
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

#define T(fmt, v, want) do { char b[64]; volatile int tc_v = (v); int n = sprintf(b, fmt, tc_v); CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want)-1)); } while (0)

TC_CASE(basic) { T("% d", 5, " 5"); T("% d", -5, "-5"); T("% d", 0, " 0"); }
TC_CASE(width) { T("[% 5d]", 5, "[    5]"); T("[% 5d]", -5, "[   -5]"); T("[%- 5d]", 5, "[ 5   ]"); }
TC_CASE(zero_pad) { T("% 05d", 5, " 0005"); T("% 05d", -5, "-0005"); }
TC_CASE(i) { T("% i", 5, " 5"); T("% i", -5, "-5"); }

int main(void) { tc_begin(); TC_RUN(basic); TC_RUN(width); TC_RUN(zero_pad); TC_RUN(i); return tc_end(); }
