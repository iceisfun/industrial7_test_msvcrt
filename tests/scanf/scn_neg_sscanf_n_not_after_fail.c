/* TC
 * id:    scn_neg_sscanf_n_not_after_fail
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.21.6.2p3 (scanning stops at first failure), p12 (%n)
 * note:  After a matching or input failure no later directive executes, so a %n after the failure point must not be stored.
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile in;

TC_CASE(positive_control) { int d = 0, n = -1, r; in = "42"; r = sscanf(in, "%d%n", &d, &n); CHECK_EQ_I(r, 1); CHECK_EQ_I(d, 42); CHECK_EQ_I(n, 2); }
TC_CASE(conv_fail) { int d = 9, n = -1, r; in = "a"; r = sscanf(in, "%d%n", &d, &n); CHECK_EQ_I(r, 0); CHECK_EQ_I(d, 9); CHECK_EQ_I(n, -1); }
TC_CASE(literal_fail) { int d = 0, n = -1, r; in = "5x"; r = sscanf(in, "%dy%n", &d, &n); CHECK_EQ_I(r, 1); CHECK_EQ_I(d, 5); CHECK_EQ_I(n, -1); }
TC_CASE(eof_first) { int d = 9, n = -1, r; in = ""; r = sscanf(in, "%d%n", &d, &n); CHECK_EQ_I(r, EOF); CHECK_EQ_I(d, 9); CHECK_EQ_I(n, -1); }
TC_CASE(eof_second) { int d = 0, e = 9, n = -1, r; in = "7"; r = sscanf(in, "%d %d%n", &d, &e, &n); CHECK_EQ_I(r, 1); CHECK_EQ_I(d, 7); CHECK_EQ_I(e, 9); CHECK_EQ_I(n, -1); }

int main(void) { tc_begin(); TC_RUN(positive_control); TC_RUN(conv_fail); TC_RUN(literal_fail); TC_RUN(eof_first); TC_RUN(eof_second); return tc_end(); }
