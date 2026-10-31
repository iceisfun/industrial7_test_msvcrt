/* TC
 * id:    scn_sscanf_suppress
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p10 (assignment suppression); MS docs scanf width/type
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(skip_int) { int a = -1, r; r = sscanf("10 20", "%*d %d", &a); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 20); }
TC_CASE(skip_str) { char s[16] = "zz"; int r; r = sscanf("foo bar", "%*s %s", s); CHECK_EQ_I(r, 1); CHECK_STR(s, "bar"); }
TC_CASE(skip_char) { char c = 'z'; int r; r = sscanf("xy", "%*c%c", &c); CHECK_EQ_I(r, 1); CHECK_EQ_I(c, 'y'); }
TC_CASE(skip_set) { int a = -1, r; r = sscanf("abcd42", "%*[a-z]%d", &a); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 42); }
TC_CASE(not_counted) { int r = sscanf("1 2 3 x", "%*d %*d %*d"); CHECK_EQ_I(r, 0); }
TC_CASE(skip_then_n) { int n = -1, r; r = sscanf("123 rest", "%*d%n", &n); CHECK_EQ_I(r, 0); CHECK_EQ_I(n, 3); }

int main(void) { tc_begin(); TC_RUN(skip_int); TC_RUN(skip_str); TC_RUN(skip_char); TC_RUN(skip_set); TC_RUN(not_counted); TC_RUN(skip_then_n); return tc_end(); }
