/* TC
 * id:    scn_sscanf_ws_skip_rules
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p8 (leading white space skipped except [, c, n); isspace in the C locale
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(numeric_skip) { int d = 0; unsigned x = 0; float f = 0; char s[16] = ""; sscanf("  \t12", "%d", &d); CHECK_EQ_I(d, 12);
    sscanf(" \n 2.5", "%f", &f); CHECK_BITS_F(f, 2.5f); sscanf("   word", "%s", s); CHECK_STR(s, "word"); sscanf("  ff", "%x", &x); CHECK_EQ_U(x, 255); }
TC_CASE(c_no_skip) { char c = 0; int r = sscanf("  q", "%c", &c); CHECK_EQ_I(r, 1); CHECK_EQ_I(c, ' '); }
TC_CASE(set_no_skip) { char s[8] = "zz"; int r = sscanf(" ab", "%[a-z]", s); CHECK_EQ_I(r, 0); }
TC_CASE(n_no_skip) { int n = -1; sscanf("   x", "%n", &n); CHECK_EQ_I(n, 0); }
TC_CASE(all_ws_chars) { int d = 0, r = sscanf("\t\n\v\f\r 8", "%d", &d); CHECK_EQ_I(r, 1); CHECK_EQ_I(d, 8); }

int main(void) { tc_begin(); TC_RUN(numeric_skip); TC_RUN(c_no_skip); TC_RUN(set_no_skip); TC_RUN(n_no_skip); TC_RUN(all_ws_chars); return tc_end(); }
