/* TC
 * id:    scn_sscanf_ws_directive
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p5 (white-space directive)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(zero_ws) { int a = 0, b = 0, r; r = sscanf("1,2", "%d ,%d", &a, &b); CHECK_EQ_I(r, 2); CHECK_EQ_I(a, 1); CHECK_EQ_I(b, 2); }
TC_CASE(many_ws) { int b = -1, r; r = sscanf("abc \n\t  def", "abc \t\n def%n", &b); CHECK_EQ_I(r, 0); CHECK_EQ_I(b, 11); }
TC_CASE(d_space_d) { int a = 0, b = 0, r; r = sscanf("1\n\t 2", "%d %d", &a, &b); CHECK_EQ_I(r, 2); CHECK_EQ_I(a, 1); CHECK_EQ_I(b, 2); }
TC_CASE(trailing_ws) { int a = 0, n = -1, r; r = sscanf("5 \n  ", "%d %n", &a, &n); CHECK_EQ_I(r, 1); CHECK_EQ_I(n, 5); }
TC_CASE(no_skip_before_comma) { int a = 0, b = 77, r; r = sscanf("1 ,2", "%d,%d", &a, &b); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 1); CHECK_EQ_I(b, 77); }

int main(void) { tc_begin(); TC_RUN(zero_ws); TC_RUN(many_ws); TC_RUN(d_space_d); TC_RUN(trailing_ws); TC_RUN(no_skip_before_comma); return tc_end(); }
