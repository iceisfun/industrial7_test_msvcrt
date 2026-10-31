/* TC
 * id:    scn_sscanf_return_count
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p16
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(all3) { int a = 0, b = 0, c = 0, r = sscanf("1 2 3", "%d %d %d", &a, &b, &c); CHECK_EQ_I(r, 3); CHECK_EQ_I(c, 3); }
TC_CASE(fail_2nd) { int a = 0, b = 9, c = 9, r = sscanf("1 x 3", "%d %d %d", &a, &b, &c); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 1); CHECK_EQ_I(b, 9); CHECK_EQ_I(c, 9); }
TC_CASE(fail_3rd) { int a = 0, b = 0, c = 9, r = sscanf("1 2 z", "%d %d %d", &a, &b, &c); CHECK_EQ_I(r, 2); CHECK_EQ_I(c, 9); }
TC_CASE(input_ends) { int a = 0, b = 0, c = 9, r = sscanf("1 2", "%d %d %d", &a, &b, &c); CHECK_EQ_I(r, 2); CHECK_EQ_I(c, 9); }
TC_CASE(suppressed) { int a = 0, b = 0, r = sscanf("1 2 3", "%d %*d %d", &a, &b); CHECK_EQ_I(r, 2); CHECK_EQ_I(a, 1); CHECK_EQ_I(b, 3); }

int main(void) { tc_begin(); TC_RUN(all3); TC_RUN(fail_2nd); TC_RUN(fail_3rd); TC_RUN(input_ends); TC_RUN(suppressed); return tc_end(); }
