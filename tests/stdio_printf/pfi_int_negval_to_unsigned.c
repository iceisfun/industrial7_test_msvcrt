/* TC
 * id:    pfi_int_negval_to_unsigned
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p8 (unsigned conversions), 6.3.1.3 conversion modulo 2^N
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(u) {
    char b[80]; volatile int a = -1, m = INT_MIN;
    sprintf(b, "%u", a); CHECK_STR(b, "4294967295");
    sprintf(b, "%u", m); CHECK_STR(b, "2147483648");
}
TC_CASE(x_o) {
    char b[80]; volatile int a = -255, c = -8;
    sprintf(b, "%x", a); CHECK_STR(b, "ffffff01");
    sprintf(b, "%X", a); CHECK_STR(b, "FFFFFF01");
    sprintf(b, "%o", c); CHECK_STR(b, "37777777770");
}
TC_CASE(long_and_i64) {
    char b[80]; volatile long l = -1L; volatile long long ll = -1LL;
    sprintf(b, "%lu", l); CHECK_STR(b, "4294967295");  /* long is 32-bit on Windows */
    sprintf(b, "%I64u", ll); CHECK_STR(b, "18446744073709551615");
}
TC_CASE(short_h) {
    char b[80]; volatile int a = -2, c = -1;
    sprintf(b, "%hu", a); CHECK_STR(b, "65534");
    sprintf(b, "%hx", c); CHECK_STR(b, "ffff");
}

int main(void) { tc_begin(); TC_RUN(u); TC_RUN(x_o); TC_RUN(long_and_i64); TC_RUN(short_h); return tc_end(); }
