/* TC
 * id:    scn_sscanf_minmax
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.21.6.2; MS docs scanf width specification (I64 prefix; long is 32 bits on Windows)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(int_ext) { int a = 0, b = 0; sscanf("-2147483648", "%d", &a); sscanf("2147483647", "%d", &b); CHECK_EQ_I(a, INT_MIN); CHECK_EQ_I(b, INT_MAX); }
TC_CASE(uint_max) { unsigned u = 0; sscanf("4294967295", "%u", &u); CHECK_EQ_U(u, UINT_MAX); }
TC_CASE(i64) { __int64 a = 0, b = 0; sscanf("-9223372036854775808", "%I64d", &a); sscanf("9223372036854775807", "%I64d", &b);
    CHECK(a == (-9223372036854775807LL - 1)); CHECK(b == 9223372036854775807LL); }
TC_CASE(u64) { unsigned __int64 u = 0; sscanf("18446744073709551615", "%I64u", &u); CHECK(u == 18446744073709551615ULL); }
TC_CASE(long32) { long a = 0, b = 0; sscanf("-2147483648", "%ld", &a); sscanf("2147483647", "%ld", &b); CHECK_EQ_I(a, -2147483647L - 1); CHECK_EQ_I(b, 2147483647L); CHECK_EQ_U(sizeof(long), 4); }

int main(void) { tc_begin(); TC_RUN(int_ext); TC_RUN(uint_max); TC_RUN(i64); TC_RUN(u64); TC_RUN(long32); return tc_end(); }
