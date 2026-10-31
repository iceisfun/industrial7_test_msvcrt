/* TC
 * id:    scn_neg_sscanf_nomatch
 * area:  scanf
 * kind:  negative
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2 p16 (return value), p9-10 (matching failure); MS docs sscanf return value
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;
TC_CASE(matching_failure_zero) { int a = 77, n; v = "abc"; n = sscanf((const char *)v, "%d", &a); CHECK_EQ_I(n, 0); CHECK_EQ_I(a, 77); }
TC_CASE(sign_only) { int a = 77, n; v = "-x"; n = sscanf((const char *)v, "%d", &a); CHECK_EQ_I(n, 0); CHECK_EQ_I(a, 77); }
TC_CASE(hex_no_digits) { unsigned a = 77; int n; v = "zz"; n = sscanf((const char *)v, "%x", &a); CHECK_EQ_I(n, 0); CHECK_EQ_U(a, 77); }
TC_CASE(empty_is_eof) { int a = 77, n; v = ""; n = sscanf((const char *)v, "%d", &a); CHECK_EQ_I(n, EOF); CHECK_EQ_I(a, 77); }
TC_CASE(only_ws_is_eof) { int a = 77, n; v = "  \t\n"; n = sscanf((const char *)v, "%d", &a); CHECK_EQ_I(n, EOF); CHECK_EQ_I(a, 77); }
TC_CASE(partial_count) { int a = 0, b = 77, n; v = "5 x"; n = sscanf((const char *)v, "%d %d", &a, &b); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 5); CHECK_EQ_I(b, 77); }
TC_CASE(literal_mismatch) { int a = 0, b = 77, n; v = "1;2"; n = sscanf((const char *)v, "%d,%d", &a, &b); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 1); CHECK_EQ_I(b, 77); }
TC_CASE(suppressed_and_n_not_counted) { int a = 0, p = -1, n; v = "10 20"; n = sscanf((const char *)v, "%*d %d%n", &a, &p); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 20); CHECK_EQ_I(p, 5); }
int main(void) { tc_begin(); TC_RUN(matching_failure_zero); TC_RUN(sign_only); TC_RUN(hex_no_digits); TC_RUN(empty_is_eof); TC_RUN(only_ws_is_eof); TC_RUN(partial_count); TC_RUN(literal_mismatch); TC_RUN(suppressed_and_n_not_counted); return tc_end(); }
