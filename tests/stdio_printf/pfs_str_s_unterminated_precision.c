/* TC
 * id:    pfs_str_s_unterminated_precision
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1p8 (s: array need not contain NUL if precision is specified)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile int p3 = 3, p2 = 2;
TC_CASE(exact) { static char a[3] = {'a','b','c'}; char b[64]; int n = sprintf(b, "%.*s", p3, (char *volatile)a); CHECK_STR(b, "abc"); CHECK_EQ_I(n, 3); }
TC_CASE(shorter) { static char a[3] = {'a','b','c'}; char b[64]; int n = sprintf(b, "%.*s", p2, (char *volatile)a); CHECK_STR(b, "ab"); CHECK_EQ_I(n, 2); }
TC_CASE(guard) {
    static char a[4] = {'a','b','c','Z'}; /* 'Z' is a guard that must not be read as output */
    char b[16]; memset(b, 0x55, sizeof b);
    int n = sprintf(b, "%.3s", (char *volatile)a);
    CHECK_EQ_I(n, 3); CHECK_MEM(b, "abc\0\x55\x55", 6);
}
int main(void) { tc_begin(); TC_RUN(exact); TC_RUN(shorter); TC_RUN(guard); return tc_end(); }
