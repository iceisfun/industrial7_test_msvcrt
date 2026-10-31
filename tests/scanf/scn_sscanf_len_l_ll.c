/* TC
 * id:    scn_sscanf_len_l_ll
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * note:  ll cases only for msvcr80+ (ll documented since VS2005; msvcrt.dll support not assumed, see scn_sscanf_lld_msvcrt_probe)
 * ref:   C11 7.21.6.2p11; MS docs scanf size specifiers (ll since VS2005)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_CASE(ld) { long a[3] = {0x1111, 0, 0x2222}; int n = sscanf("2147483647", "%ld", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_I(sizeof(long), 4); CHECK_EQ_I(a[1], 2147483647L); CHECK_EQ_I(a[0], 0x1111); CHECK_EQ_I(a[2], 0x2222); }
#if TC_AT_LEAST(80)
TC_CASE(lld) { long long a[3] = {0x1111, 0, 0x2222}; int n = sscanf("9223372036854775807", "%lld", &a[1]); CHECK_EQ_I(n, 1); CHECK(a[1] == 9223372036854775807LL); CHECK_EQ_I(a[0], 0x1111); CHECK_EQ_I(a[2], 0x2222); }
#endif
#if TC_AT_LEAST(80)
TC_CASE(llx) { unsigned long long a[3] = {0x1111, 0, 0x2222}; int n = sscanf("ffffffffffffffff", "%llx", &a[1]); CHECK_EQ_I(n, 1); CHECK(a[1] == 0xffffffffffffffffull); CHECK_EQ_U(a[0], 0x1111); CHECK_EQ_U(a[2], 0x2222); }
#endif
#if TC_AT_LEAST(80)
TC_CASE(llu) { unsigned long long a[3] = {0x1111, 0, 0x2222}; int n = sscanf("18446744073709551615", "%llu", &a[1]); CHECK_EQ_I(n, 1); CHECK(a[1] == 18446744073709551615ull); CHECK_EQ_U(a[2], 0x2222); }
#endif
TC_CASE(lo) { unsigned long a[3] = {0x1111, 0, 0x2222}; int n = sscanf("37777777777", "%lo", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_U(a[1], 0xffffffffu); CHECK_EQ_U(a[0], 0x1111); CHECK_EQ_U(a[2], 0x2222); }

int main(void) { tc_begin(); TC_RUN(ld); 
#if TC_AT_LEAST(80)
  TC_RUN(lld); TC_RUN(llx); TC_RUN(llu);
#endif
   TC_RUN(lo); return tc_end(); }
