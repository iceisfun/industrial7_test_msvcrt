/* TC
 * id:    pfi_int_snprintf_fits
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf (returns chars written, NUL when count > length)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(plain) { char b[64]; volatile int v = -123; int n = _snprintf(b, sizeof b, "%d", v); CHECK_EQ_I(n, 4); CHECK_STR(b, "-123"); }
TC_CASE(matrix) {
    char b[64]; volatile int v = 42; int n;
    n = _snprintf(b, sizeof b, "[%6d][%-6d][%06d][%+d][% d][%.4d][%#x]", v, v, v, v, v, v, v);
    CHECK_STR(b, "[    42][42    ][000042][+42][ 42][0042][0x2a]");
    CHECK_EQ_I(n, 46);
}
TC_CASE(exact_fit) {
    char b[8]; volatile int v = 1234; int n; int i;
    for (i = 0; i < 8; i++) b[i] = 'Z';
    n = _snprintf(b, 5, "%d", v); CHECK_EQ_I(n, 4); CHECK_STR(b, "1234"); CHECK_EQ_I(b[5], 'Z');
}
TC_CASE(i64_ll) {
    char b[64]; volatile long long v = -9223372036854775807LL - 1; int n;
    n = _snprintf(b, sizeof b, "%I64d", v); CHECK_EQ_I(n, 20); CHECK_STR(b, "-9223372036854775808");
#if TC_AT_LEAST(80)
    n = _snprintf(b, sizeof b, "%lld", v); CHECK_EQ_I(n, 20); CHECK_STR(b, "-9223372036854775808");
#endif
}

int main(void) { tc_begin(); TC_RUN(plain); TC_RUN(matrix); TC_RUN(exact_fit); TC_RUN(i64_ll); return tc_end(); }
