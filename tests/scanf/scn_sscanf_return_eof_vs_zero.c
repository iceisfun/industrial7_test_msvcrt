/* TC
 * id:    scn_sscanf_return_eof_vs_zero
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p16 (EOF if input failure before first conversion, else count)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(empty) { int a = 5, r = sscanf("", "%d", &a); CHECK_EQ_I(r, EOF); CHECK_EQ_I(a, 5); }
TC_CASE(only_ws) { int a = 5, r = sscanf("   ", "%d", &a); CHECK_EQ_I(r, EOF); }
TC_CASE(matching_failure) { int a = 5, r = sscanf("abc", "%d", &a); CHECK_EQ_I(r, 0); CHECK_EQ_I(a, 5); }
TC_CASE(literal_mismatch) { int r = sscanf("abc", "xyz"); CHECK_EQ_I(r, 0); }
TC_CASE(eof_only_before_first) { int a = 0, b = 5, r = sscanf("7", "%d %d", &a, &b); CHECK_EQ_I(r, 1); CHECK_EQ_I(b, 5); }

int main(void) { tc_begin(); TC_RUN(empty); TC_RUN(only_ws); TC_RUN(matching_failure); TC_RUN(literal_mismatch); TC_RUN(eof_only_before_first); return tc_end(); }
