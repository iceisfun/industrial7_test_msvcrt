/* TC
 * id:    scn_neg_sscanf_eof_after_assign
 * area:  scanf
 * kind:  negative
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p16 (EOF only if no conversion completed; otherwise the number assigned)
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile vin;
TC_CASE(one_of_two) { int a = 0, b = 9, n; vin = "1"; n = sscanf((const char *)vin, "%d %d", &a, &b); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 1); CHECK_EQ_I(b, 9); }
TC_CASE(trailing_space) { int a = 0, b = 9, n; vin = "1 "; n = sscanf((const char *)vin, "%d %d", &a, &b); CHECK_EQ_I(n, 1); CHECK_EQ_I(b, 9); }
TC_CASE(two_of_three) { int a = 0, b = 0, c = 9, n; vin = "1 2"; n = sscanf((const char *)vin, "%d %d %d", &a, &b, &c); CHECK_EQ_I(n, 2); CHECK_EQ_I(a, 1); CHECK_EQ_I(b, 2); CHECK_EQ_I(c, 9); }
int main(void) { tc_begin(); TC_RUN(one_of_two); TC_RUN(trailing_space); TC_RUN(two_of_three); return tc_end(); }
