/* TC
 * id:    scn_sscanf_octal_o
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
TC_CASE(sevens) { unsigned a = 0; int n; v = "777"; n = sscanf((const char *)v, "%o", &a); CHECK_EQ_I(n, 1); CHECK_EQ_U(a, 511); }
TC_CASE(leading_zero) { unsigned a = 0; int n; v = "017"; n = sscanf((const char *)v, "%o", &a); CHECK_EQ_I(n, 1); CHECK_EQ_U(a, 15); }
TC_CASE(stops_at_8) { unsigned a = 0; int n, pos = -1; char c = 0; v = "128"; n = sscanf((const char *)v, "%o%c%n", &a, &c, &pos); CHECK_EQ_I(n, 2); CHECK_EQ_U(a, 10); CHECK_EQ_I(c, '8'); CHECK_EQ_I(pos, 3); }
TC_CASE(stops_at_9) { unsigned a = 0; int n; char c = 0; v = "79"; n = sscanf((const char *)v, "%o%c", &a, &c); CHECK_EQ_I(n, 2); CHECK_EQ_U(a, 7); CHECK_EQ_I(c, '9'); }
TC_CASE(width) { unsigned a = 0, b = 0; int n; v = "7654"; n = sscanf((const char *)v, "%2o%o", &a, &b); CHECK_EQ_I(n, 2); CHECK_EQ_U(a, 62); CHECK_EQ_U(b, 44); }
int main(void) { tc_begin(); TC_RUN(sevens); TC_RUN(leading_zero); TC_RUN(stops_at_8); TC_RUN(stops_at_9); TC_RUN(width); return tc_end(); }
