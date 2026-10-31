/* TC
 * id:    pfs_str_c_basic
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1; MS docs printf type field characters
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile int A = 'A', B = 'B', C = 'C';
TC_CASE(plain) { char b[64]; int n = sprintf(b, "%c", A); CHECK_STR(b, "A"); CHECK_EQ_I(n, 1); }
TC_CASE(right) { char b[64]; int n = sprintf(b, "[%5c]", A); CHECK_STR(b, "[    A]"); CHECK_EQ_I(n, 7); }
TC_CASE(left) { char b[64]; sprintf(b, "[%-5c]", A); CHECK_STR(b, "[A    ]"); }
TC_CASE(adjacent) { char b[64]; int n = sprintf(b, "%c%c%c", A, B, C); CHECK_STR(b, "ABC"); CHECK_EQ_I(n, 3); }
TC_CASE(ret_width) { char b[64]; CHECK_EQ_I(sprintf(b, "%c", A), 1); CHECK_EQ_I(sprintf(b, "%5c", A), 5); }
int main(void) { tc_begin(); TC_RUN(plain); TC_RUN(right); TC_RUN(left); TC_RUN(adjacent); TC_RUN(ret_width); return tc_end(); }
