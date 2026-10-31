/* TC
 * id:    scn_sscanf_n_failure_unchanged
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.2 (%n); MS docs scanf: %n not counted in return
 * note:  
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

static volatile const char *v;
TC_CASE(match_fail) { int a = 99, pos = -7; int n; v = "x"; n = sscanf((const char *)v, "%d%n", &a, &pos); CHECK_EQ_I(n, 0); CHECK_EQ_I(a, 99); CHECK_EQ_I(pos, -7); }
TC_CASE(literal_mismatch) { int a = 0, pos = -7; int n; v = "12 x"; n = sscanf((const char *)v, "%d y%n", &a, &pos); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 12); CHECK_EQ_I(pos, -7); }
TC_CASE(reached) { int a = 0, pos = -7; int n; v = "12"; n = sscanf((const char *)v, "%d%n", &a, &pos); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 12); CHECK_EQ_I(pos, 2); }
TC_CASE(n_not_counted) { int pos = -7; int n; v = "abc"; n = sscanf((const char *)v, "abc%n", &pos); CHECK_EQ_I(n, 0); CHECK_EQ_I(pos, 3); }
int main(void) { tc_begin(); TC_RUN(match_fail); TC_RUN(literal_mismatch); TC_RUN(reached); TC_RUN(n_not_counted); return tc_end(); }
