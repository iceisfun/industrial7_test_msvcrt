/* TC
 * id:    scn_sscanf_lld_msvcrt_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs scanf size prefixes
 * note:  PROBE: whether msvcrt.dll sscanf understands the C99 "ll" prefix (Win7-era msvcrt.dll); expected to store a full 64-bit value.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(lld) { long long a[3] = {0x1111, 0, 0x2222}; int n = sscanf("9223372036854775807", "%lld", &a[1]); CHECK_EQ_I(n, 1); CHECK(a[1] == 9223372036854775807LL); CHECK_EQ_I(a[0], 0x1111); CHECK_EQ_I(a[2], 0x2222); }

int main(void) { tc_begin(); TC_RUN(lld); return tc_end(); }
