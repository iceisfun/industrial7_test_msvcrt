/* TC
 * id:    pfi_int_flag_space_plus_combo
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

TC_CASE(space_plus) { T("%+ d", 5, "+5"); T("%+ d", 0, "+0"); }
TC_CASE(plus_space) { T("% +d", 5, "+5"); T("% +d", 0, "+0"); }
TC_CASE(negative) { T("% +d", -5, "-5"); T("%+ d", -5, "-5"); }
TC_CASE(minus_width) { T("%+-6d|", 5, "+5    |"); T("%+-6d|", -5, "-5    |"); T("% -6d|", 5, " 5    |"); }
TC_CASE(zero_combo) { T("%+ 06d", 5, "+00005"); T("% +06d", -5, "-00005"); }

int main(void) { tc_begin(); TC_RUN(space_plus); TC_RUN(plus_space); TC_RUN(negative); TC_RUN(minus_width); TC_RUN(zero_combo); return tc_end(); }
