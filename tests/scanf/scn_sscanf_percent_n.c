/* TC
 * id:    scn_sscanf_percent_n
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p12 (%n), p16; MS docs scanf type field characters
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(not_counted) { int a = 0, n = -1, r; r = sscanf("12 34", "%d%n", &a, &n); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 12); CHECK_EQ_I(n, 2); }
TC_CASE(after_d) { int a = 0, n = -1; sscanf("  -987xyz", "%d%n", &a, &n); CHECK_EQ_I(a, -987); CHECK_EQ_I(n, 6); }
TC_CASE(at_start) { int n = -1, r; r = sscanf("   abc", "%n", &n); CHECK_EQ_I(n, 0); CHECK_EQ_I(r, 0); }
TC_CASE(counts_ws) { int a = 0, b = 0, n = -1, r; r = sscanf(" 1\t\n 22  ", "%d %d%n", &a, &b, &n); CHECK_EQ_I(r, 2); CHECK_EQ_I(a, 1); CHECK_EQ_I(b, 22); CHECK_EQ_I(n, 7); }
TC_CASE(long_size) { long v[2]; v[0] = v[1] = -1; sscanf("abc", "abc%ln", &v[0]); CHECK_EQ_I(v[0], 3); CHECK_EQ_I(v[1], -1); }

int main(void) { tc_begin(); TC_RUN(not_counted); TC_RUN(after_d); TC_RUN(at_start); TC_RUN(counts_ws); TC_RUN(long_size); return tc_end(); }
