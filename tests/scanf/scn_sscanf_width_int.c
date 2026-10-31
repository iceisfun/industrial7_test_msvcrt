/* TC
 * id:    scn_sscanf_width_int
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2; MS docs scanf type field characters
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

static volatile const char *v;
TC_CASE(two_two) { int a = 0, b = 0, n; v = "12345"; n = sscanf((const char *)v, "%2d%2d", &a, &b); CHECK_EQ_I(n, 2); CHECK_EQ_I(a, 12); CHECK_EQ_I(b, 34); }
TC_CASE(sign_counts) { int a = 0, b = 0, n; v = "-1234"; n = sscanf((const char *)v, "%3d%d", &a, &b); CHECK_EQ_I(n, 2); CHECK_EQ_I(a, -12); CHECK_EQ_I(b, 34); }
TC_CASE(one) { int a = 0, b = 0, c = 0, n; v = "789"; n = sscanf((const char *)v, "%1d%1d%1d", &a, &b, &c); CHECK_EQ_I(n, 3); CHECK_EQ_I(a, 7); CHECK_EQ_I(b, 8); CHECK_EQ_I(c, 9); }
TC_CASE(hex_prefix_counts) { unsigned a = 0, b = 0; int n; v = "0x1234"; n = sscanf((const char *)v, "%4x%x", &a, &b); CHECK_EQ_I(n, 2); CHECK_EQ_U(a, 0x12); CHECK_EQ_U(b, 0x34); }
TC_CASE(ws_not_counted) { int a = 0, b = 0, n; v = "   12345"; n = sscanf((const char *)v, "%2d%d", &a, &b); CHECK_EQ_I(n, 2); CHECK_EQ_I(a, 12); CHECK_EQ_I(b, 345); }
int main(void) { tc_begin(); TC_RUN(two_two); TC_RUN(sign_counts); TC_RUN(one); TC_RUN(hex_prefix_counts); TC_RUN(ws_not_counted); return tc_end(); }
