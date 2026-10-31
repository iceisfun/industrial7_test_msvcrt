/* TC
 * id:    scn_sscanf_int_d
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
TC_CASE(signs) { int a = 9, b = 9, c = 9, n; v = "42 -42 +42"; n = sscanf((const char *)v, "%d %d %d", &a, &b, &c); CHECK_EQ_I(n, 3); CHECK_EQ_I(a, 42); CHECK_EQ_I(b, -42); CHECK_EQ_I(c, 42); }
TC_CASE(leading_ws) { int a = 0, n; v = " \t\n\r  17"; n = sscanf((const char *)v, "%d", &a); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 17); }
TC_CASE(stop_nondigit) { int a = 0, n, pos = -1; char c = 0; v = "123abc"; n = sscanf((const char *)v, "%d%c%n", &a, &c, &pos); CHECK_EQ_I(n, 2); CHECK_EQ_I(a, 123); CHECK_EQ_I(c, 'a'); CHECK_EQ_I(pos, 4); }
TC_CASE(limits) { int a = 0, b = 0, n; v = "2147483647 -2147483648"; n = sscanf((const char *)v, "%d %d", &a, &b); CHECK_EQ_I(n, 2); CHECK_EQ_I(a, INT_MAX); CHECK_EQ_I(b, INT_MIN); }
TC_CASE(zeros) { int a = 5, b = 5, n; v = "0 -0"; n = sscanf((const char *)v, "%d %d", &a, &b); CHECK_EQ_I(n, 2); CHECK_EQ_I(a, 0); CHECK_EQ_I(b, 0); }
int main(void) { tc_begin(); TC_RUN(signs); TC_RUN(leading_ws); TC_RUN(stop_nondigit); TC_RUN(limits); TC_RUN(zeros); return tc_end(); }
