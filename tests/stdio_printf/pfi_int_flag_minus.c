/* TC
 * id:    pfi_int_flag_minus
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

#define TU(fmt, v, want) do { char b[64]; volatile unsigned tc_v = (v); int n = sprintf(b, fmt, tc_v); CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want)-1)); } while (0)
TC_CASE(d) { T("%-5d|", 42, "42   |"); T("%-5d|", 0, "0    |"); }
TC_CASE(uxo) { TU("%-5u|", 42u, "42   |"); TU("%-5x|", 255u, "ff   |"); TU("%-5o|", 8u, "10   |"); TU("%-5X|", 255u, "FF   |"); }
TC_CASE(longer) { T("%-3d|", 123456, "123456|"); T("%-3d|", 1234, "1234|"); T("%-1d|", 12, "12|"); }
TC_CASE(negative) { T("%-6d|", -42, "-42   |"); T("%-4d|", -42, "-42 |"); }
TC_CASE(minus_beats_zero) { T("%-05d|", 42, "42   |"); T("%0-5d|", -42, "-42  |"); }

int main(void) { tc_begin(); TC_RUN(d); TC_RUN(uxo); TC_RUN(longer); TC_RUN(negative); TC_RUN(minus_beats_zero); return tc_end(); }
