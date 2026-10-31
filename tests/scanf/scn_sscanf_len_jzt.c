/* TC
 * id:    scn_sscanf_len_jzt
 * area:  scanf
 * funcs: sscanf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs scanf width specification (j, z, t added in VS2015 UCRT); C11 7.21.6.2p11
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

#include <stddef.h>
#include <stdint.h>

TC_CASE(jd) { intmax_t a[3] = {0x1111, 0, 0x2222}; int n = sscanf("-9223372036854775807", "%jd", &a[1]); CHECK_EQ_I(n, 1); CHECK(a[1] == -9223372036854775807LL); CHECK_EQ_I(a[0], 0x1111); CHECK_EQ_I(a[2], 0x2222); }
TC_CASE(ju) { uintmax_t a[3] = {0x1111, 0, 0x2222}; int n = sscanf("18446744073709551615", "%ju", &a[1]); CHECK_EQ_I(n, 1); CHECK(a[1] == 0xffffffffffffffffull); CHECK_EQ_U(a[2], 0x2222); }
TC_CASE(zu) { size_t a[3] = {0x1111, 0, 0x2222}; int n = sscanf("123456", "%zu", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_U(a[1], 123456); CHECK_EQ_U(a[0], 0x1111); CHECK_EQ_U(a[2], 0x2222); }
TC_CASE(zd) { ptrdiff_t a[3] = {0x1111, 0, 0x2222}; int n = sscanf("-77", "%zd", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_I(a[1], -77); CHECK_EQ_I(a[0], 0x1111); CHECK_EQ_I(a[2], 0x2222); }
TC_CASE(td) { ptrdiff_t a[3] = {0x1111, 0, 0x2222}; int n = sscanf("-4096", "%td", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_I(a[1], -4096); CHECK_EQ_I(a[0], 0x1111); CHECK_EQ_I(a[2], 0x2222); }
TC_CASE(tx_wide) { ptrdiff_t a[3] = {0x1111, 0, 0x2222}; int n = sscanf("7fffffff", "%tx", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_I(a[1], 0x7fffffff); CHECK_EQ_I(a[2], 0x2222); }

int main(void) { tc_begin(); TC_RUN(jd); TC_RUN(ju); TC_RUN(zu); TC_RUN(zd); TC_RUN(td); TC_RUN(tx_wide); return tc_end(); }
