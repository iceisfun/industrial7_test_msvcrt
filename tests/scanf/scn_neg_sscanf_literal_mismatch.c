/* TC
 * id:    scn_neg_sscanf_literal_mismatch
 * area:  scanf
 * kind:  negative
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p5-6 (ordinary characters must match exactly; mismatch is a matching failure), p8 (%% matches a single %)
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile vin;
TC_CASE(prefix_mismatch) { int x = 77, n; vin = "x=5"; n = sscanf((const char *)vin, "y=%d", &x); CHECK_EQ_I(n, 0); CHECK_EQ_I(x, 77); }
TC_CASE(separator_mismatch) { int a = 0, b = 77, n; vin = "1,2"; n = sscanf((const char *)vin, "%d;%d", &a, &b); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 1); CHECK_EQ_I(b, 77); }
TC_CASE(case_mismatch) { int x = 77, n; vin = "ID=5"; n = sscanf((const char *)vin, "id=%d", &x); CHECK_EQ_I(n, 0); CHECK_EQ_I(x, 77); }
TC_CASE(percent_vs_a) { int x = 77, n; vin = "a5"; n = sscanf((const char *)vin, "%%%d", &x); CHECK_EQ_I(n, 0); CHECK_EQ_I(x, 77); }
TC_CASE(percent_match) { int x = 0, n; vin = "%5"; n = sscanf((const char *)vin, "%%%d", &x); CHECK_EQ_I(n, 1); CHECK_EQ_I(x, 5); }
int main(void) { tc_begin(); TC_RUN(prefix_mismatch); TC_RUN(separator_mismatch); TC_RUN(case_mismatch); TC_RUN(percent_vs_a); TC_RUN(percent_match); return tc_end(); }
