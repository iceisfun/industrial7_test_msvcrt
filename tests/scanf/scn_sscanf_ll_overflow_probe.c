/* TC
 * id:    scn_sscanf_ll_overflow_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.21.6.2p10; MS docs scanf size prefixes (I64)
 * note:  PROBE: 64-bit wrap. Uses %I64 prefix, supported by every MS CRT.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

static volatile const char *v;
TC_CASE(lld) { long long a = 5; int n; v = "9223372036854775808"; n = sscanf((const char *)v, "%I64d", &a); CHECK_EQ_I(n, 1); CHECK(a == LLONG_MIN); }
TC_CASE(llu) { unsigned long long a = 5; int n; v = "18446744073709551616"; n = sscanf((const char *)v, "%I64u", &a); CHECK_EQ_I(n, 1); CHECK(a == 0); }
int main(void) { tc_begin(); TC_RUN(lld); TC_RUN(llu); return tc_end(); }
