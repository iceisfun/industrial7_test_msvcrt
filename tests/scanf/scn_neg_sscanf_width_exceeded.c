/* TC
 * id:    scn_neg_sscanf_width_exceeded
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.21.6.2p4 (maximum field width), p12
 * note:  A field width truncates the input field; the remainder stays unread. 
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile in;

TC_CASE(d_width2) { int v = 0, w = 0, n = -1, r; in = "12345"; r = sscanf(in, "%2d%n%d", &v, &n, &w); CHECK_EQ_I(r, 2); CHECK_EQ_I(v, 12); CHECK_EQ_I(n, 2); CHECK_EQ_I(w, 345); }
TC_CASE(s_width3) { char a[8] = "", b[8] = ""; int r, n = -1; in = "abcdef"; r = sscanf(in, "%3s%n%s", a, &n, b); CHECK_EQ_I(r, 2); CHECK_STR(a, "abc"); CHECK_EQ_I(n, 3); CHECK_STR(b, "def"); }
TC_CASE(c_width2_exact) { char a[4] = {'x','x','x','x'}; int r; in = "abcd"; r = sscanf(in, "%2c", a); CHECK_EQ_I(r, 1); CHECK(a[0]=='a' && a[1]=='b' && a[2]=='x' && a[3]=='x'); }
TC_CASE(width2_signed) { int v = 0, r; in = "-5"; r = sscanf(in, "%2d", &v); CHECK_EQ_I(r, 1); CHECK_EQ_I(v, -5); }

int main(void) { tc_begin(); TC_RUN(d_width2); TC_RUN(s_width3); TC_RUN(c_width2_exact); TC_RUN(width2_signed); return tc_end(); }
