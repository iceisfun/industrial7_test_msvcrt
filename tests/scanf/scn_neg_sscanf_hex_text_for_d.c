/* TC
 * id:    scn_neg_sscanf_hex_text_for_d
 * area:  scanf
 * kind:  negative
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p12 (%d: subject sequence per strtol with base 10; no 0x/octal prefix handling)
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile vin;
TC_CASE(hex_text) { int d = 9, n = -1, r; vin = "0x1F"; r = sscanf((const char *)vin, "%d%n", &d, &n); CHECK_EQ_I(r, 1); CHECK_EQ_I(d, 0); CHECK_EQ_I(n, 1); }
TC_CASE(leading_zero) { int d = 0, r; vin = "017"; r = sscanf((const char *)vin, "%d", &d); CHECK_EQ_I(r, 1); CHECK_EQ_I(d, 17); }
TC_CASE(underscore) { int d = 0, n = -1, r; vin = "1_000"; r = sscanf((const char *)vin, "%d%n", &d, &n); CHECK_EQ_I(r, 1); CHECK_EQ_I(d, 1); CHECK_EQ_I(n, 1); }
int main(void) { tc_begin(); TC_RUN(hex_text); TC_RUN(leading_zero); TC_RUN(underscore); return tc_end(); }
