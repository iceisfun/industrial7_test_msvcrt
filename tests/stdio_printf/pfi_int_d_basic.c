/* TC
 * id:    pfi_int_d_basic
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

TC_CASE(pos_neg_zero) { T("%d", 123, "123"); T("%d", -123, "-123"); T("%d", 0, "0"); T("%d", 1, "1"); T("%d", -1, "-1"); }
TC_CASE(retval) { char b[64]; volatile int v = -4567; int n = sprintf(b, "ab%dcd", v); CHECK_STR(b, "ab-4567cd"); CHECK_EQ_I(n, 9); }
TC_CASE(limits) { T("%d", INT_MAX, "2147483647"); T("%d", INT_MIN, "-2147483648"); }
TC_CASE(width_prec) { T("[%6d]", 42, "[    42]"); T("[%.4d]", -42, "[-0042]"); T("[%.0d]", 0, "[]"); T("[%.0d]", 1, "[1]"); }
TC_CASE(zero_flag_prec) { T("[%08d]", -42, "[-0000042]"); T("[%08.3d]", 5, "[     005]"); T("[%08.3d]", -5, "[    -005]"); }

int main(void) { tc_begin(); TC_RUN(pos_neg_zero); TC_RUN(retval); TC_RUN(limits); TC_RUN(width_prec); TC_RUN(zero_flag_prec); return tc_end(); }
