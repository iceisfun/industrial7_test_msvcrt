/* TC
 * id:    scn_sscanf_lld
 * area:  scanf
 * funcs: sscanf
 * crt:   >=140
 * tier:  p0
 * conf:  documented
 * ref:   MS docs scanf width specification: ll prefix
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(lld) { long long a = 0, b = 0; sscanf("-9223372036854775808", "%lld", &a); sscanf("9223372036854775807", "%lld", &b);
    CHECK(a == (-9223372036854775807LL - 1)); CHECK(b == 9223372036854775807LL); }
TC_CASE(llu) { unsigned long long u = 0; sscanf("18446744073709551615", "%llu", &u); CHECK(u == 18446744073709551615ULL); }

int main(void) { tc_begin(); TC_RUN(lld); TC_RUN(llu); return tc_end(); }
