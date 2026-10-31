/* TC
 * id:    scn_neg_sscanf_float_bad_exponent_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.21.6.2p9/p12 (input item is the longest prefix that is, or can be extended to, a valid sequence)
 * note:  PROBES a dangling exponent ("1e", "1e+", "1ex"). Strict C says the item "1e" is not a valid sequence (matching failure, return 0);
 *        the MS CRT is believed to return 1 with value 1.0 and to have consumed the 'e' (and sign). Asserts the MS belief: ret 1, value 1.0.
 *        Consumed-count checks are in separate cases.
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile in;

TC_CASE(e_only_value)   { double d = 7.0; int r; in = "1e"; r = sscanf(in, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 1.0); }
TC_CASE(e_only_n)       { double d = 7.0; int r, n = -1; in = "1e"; r = sscanf(in, "%lf%n", &d, &n); CHECK_EQ_I(r, 1); CHECK_EQ_I(n, 2); }
TC_CASE(e_plus_value)   { double d = 7.0; int r; in = "1e+"; r = sscanf(in, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 1.0); }
TC_CASE(e_plus_n)       { double d = 7.0; int r, n = -1; in = "1e+"; r = sscanf(in, "%lf%n", &d, &n); CHECK_EQ_I(r, 1); CHECK_EQ_I(n, 3); }
TC_CASE(ex_value)       { double d = 7.0; int r; in = "1ex"; r = sscanf(in, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 1.0); }
TC_CASE(ex_next_char)   { double d = 7.0; char c = 0; int r; in = "1ex"; r = sscanf(in, "%lf%c", &d, &c); CHECK_EQ_I(r, 2); CHECK(c == 'x'); }

int main(void) { tc_begin(); TC_RUN(e_only_value); TC_RUN(e_only_n); TC_RUN(e_plus_value); TC_RUN(e_plus_n); TC_RUN(ex_value); TC_RUN(ex_next_char); return tc_end(); }
