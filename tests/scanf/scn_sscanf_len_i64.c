/* TC
 * id:    scn_sscanf_len_i64
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs scanf width specification / size prefixes (I64)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_CASE(I64d) { __int64 a[3] = {0x1111, 0, 0x2222}; int n = sscanf("-9223372036854775808", "%I64d", &a[1]); CHECK_EQ_I(n, 1); CHECK(a[1] == (-9223372036854775807LL - 1)); CHECK_EQ_I(a[0], 0x1111); CHECK_EQ_I(a[2], 0x2222); }
TC_CASE(I64u) { unsigned __int64 a[3] = {0x1111, 0, 0x2222}; int n = sscanf("18446744073709551615", "%I64u", &a[1]); CHECK_EQ_I(n, 1); CHECK(a[1] == 0xffffffffffffffffull); CHECK_EQ_U(a[0], 0x1111); CHECK_EQ_U(a[2], 0x2222); }
TC_CASE(I64x) { unsigned __int64 a[3] = {0x1111, 0, 0x2222}; int n = sscanf("123456789abcdef0", "%I64x", &a[1]); CHECK_EQ_I(n, 1); CHECK(a[1] == 0x123456789abcdef0ull); CHECK_EQ_U(a[2], 0x2222); }
TC_CASE(I64o) { unsigned __int64 a[3] = {0x1111, 0, 0x2222}; int n = sscanf("1777777777777777777777", "%I64o", &a[1]); CHECK_EQ_I(n, 1); CHECK(a[1] == 0xffffffffffffffffull); CHECK_EQ_U(a[0], 0x1111); CHECK_EQ_U(a[2], 0x2222); }

int main(void) { tc_begin(); TC_RUN(I64d); TC_RUN(I64u); TC_RUN(I64x); TC_RUN(I64o); return tc_end(); }
