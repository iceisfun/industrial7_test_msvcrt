/* TC
 * id:    pfi_int_minmax_int64
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

TC_CASE(dec) {
    char b[80]; volatile long long mn = (-9223372036854775807LL - 1), mx = 9223372036854775807LL;
    sprintf(b, "%I64d", mn); CHECK_STR(b, "-9223372036854775808");
    sprintf(b, "%I64d", mx); CHECK_STR(b, "9223372036854775807");
}
TC_CASE(unsigned_max) {
    char b[80]; volatile unsigned long long m = 18446744073709551615ULL;
    sprintf(b, "%I64u", m); CHECK_STR(b, "18446744073709551615");
}
TC_CASE(hex_oct) {
    char b[80]; volatile unsigned long long h = 0x8000000000000000ULL, m = 18446744073709551615ULL;
    sprintf(b, "%I64x", h); CHECK_STR(b, "8000000000000000");
    sprintf(b, "%I64X", m); CHECK_STR(b, "FFFFFFFFFFFFFFFF");
    sprintf(b, "%I64o", m); CHECK_STR(b, "1777777777777777777777");
    sprintf(b, "%#I64x", h); CHECK_STR(b, "0x8000000000000000");
}
TC_CASE(plus_prec_min) {
    char b[80]; volatile long long mn = (-9223372036854775807LL - 1); int n;
    n = sprintf(b, "%+.25I64d", mn); CHECK_STR(b, "-0000009223372036854775808"); CHECK_EQ_I(n, 26);
    n = sprintf(b, "%30I64d", mn); CHECK_EQ_I(n, 30); CHECK_STR(b, "          -9223372036854775808");
}

int main(void) { tc_begin(); TC_RUN(dec); TC_RUN(unsigned_max); TC_RUN(hex_oct); TC_RUN(plus_prec_min); return tc_end(); }
