/* TC
 * id:    pfi_int_multi_arg_order
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1; MS docs printf size prefixes (I64)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(sequence) {
    char b[160]; volatile int d = -1; volatile unsigned u = 2; volatile unsigned x = 255, o = 8;
    volatile long l = -4; volatile short h = -5; volatile long long q = -6;
    sprintf(b, "%d %u %x %o %ld %hd %I64d", d, u, x, o, l, (int)h, q);
    CHECK_STR(b, "-1 2 ff 10 -4 -5 -6");
}
TC_CASE(literals) {
    char b[160]; volatile int a = 10, c = 20;
    sprintf(b, "a=%d%%, c=%d%% done", a, c); CHECK_STR(b, "a=10%, c=20% done");
}
TC_CASE(adjacent) {
    char b[160]; volatile int a = 1, c = 2; volatile unsigned x = 0xab;
    sprintf(b, "%d%d%x%X", a, c, x, x); CHECK_STR(b, "12abAB");
}

int main(void) { tc_begin(); TC_RUN(sequence); TC_RUN(literals); TC_RUN(adjacent); return tc_end(); }
