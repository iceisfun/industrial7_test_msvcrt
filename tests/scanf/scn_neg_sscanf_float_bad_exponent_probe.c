/* TC
 * id:    scn_neg_sscanf_float_bad_exponent_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.21.6.2p9/p12 (input item is the longest prefix that is, or can be extended to, a valid sequence)
 * note:  PROBES a dangling exponent ("1e", "1e+", "1ex"). Strict C says the item "1e" is not a valid sequence (matching failure, return 0);
 *        the MS CRT is believed to return 1 with value 1.0 and to have consumed the 'e' (and sign). Asserts the MS belief: ret 1, value 1.0.
 *        Consumed-count checks are in separate cases.
 *        Win10 22H2 ucrtbase (observed 2026-10-31, stable over 2 runs): a dangling exponent ("1e", "1e+") fails the whole conversion: r==EOF at end of input (r==0 for "1ex"), the double and %n stay untouched and no later conversion runs.
 *        The old expectation (value 1.0 accepted, exponent ignored) was wrong. Conforms to C11 7.21.6.2p9 and EXAMPLE p20 (a dangling exponent is not a matching sequence; '100ergs' with %f fails); Windows returns EOF (not 0) when the input ends inside the item. Class A: the old belief was a legacy-msvcrt reading.
 *        Gated: the observed values apply to ucrtbase only (#ifdef TC_UCRT); other CRTs keep the original (unverified, probe) expectation until they are run in the VM.
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile in;

#ifdef TC_UCRT
TC_CASE(e_only_value)   { double d = 7.0; int r; in = "1e"; r = sscanf(in, "%lf", &d); CHECK_EQ_I(r, EOF); CHECK_BITS_D(d, 7.0); }
TC_CASE(e_only_n)       { double d = 7.0; int r, n = -1; in = "1e"; r = sscanf(in, "%lf%n", &d, &n); CHECK_EQ_I(r, EOF); CHECK_EQ_I(n, -1); }
TC_CASE(e_plus_value)   { double d = 7.0; int r; in = "1e+"; r = sscanf(in, "%lf", &d); CHECK_EQ_I(r, EOF); CHECK_BITS_D(d, 7.0); }
TC_CASE(e_plus_n)       { double d = 7.0; int r, n = -1; in = "1e+"; r = sscanf(in, "%lf%n", &d, &n); CHECK_EQ_I(r, EOF); CHECK_EQ_I(n, -1); }
TC_CASE(ex_value)       { double d = 7.0; int r; in = "1ex"; r = sscanf(in, "%lf", &d); CHECK_EQ_I(r, 0); CHECK_BITS_D(d, 7.0); }
TC_CASE(ex_next_char)   { double d = 7.0; char c = 0; int r; in = "1ex"; r = sscanf(in, "%lf%c", &d, &c); CHECK_EQ_I(r, 0); CHECK(c == 0); }
#else
TC_CASE(e_only_value)   { double d = 7.0; int r; in = "1e"; r = sscanf(in, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 1.0); }
TC_CASE(e_only_n)       { double d = 7.0; int r, n = -1; in = "1e"; r = sscanf(in, "%lf%n", &d, &n); CHECK_EQ_I(r, 1); CHECK_EQ_I(n, 2); }
TC_CASE(e_plus_value)   { double d = 7.0; int r; in = "1e+"; r = sscanf(in, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 1.0); }
TC_CASE(e_plus_n)       { double d = 7.0; int r, n = -1; in = "1e+"; r = sscanf(in, "%lf%n", &d, &n); CHECK_EQ_I(r, 1); CHECK_EQ_I(n, 3); }
TC_CASE(ex_value)       { double d = 7.0; int r; in = "1ex"; r = sscanf(in, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 1.0); }
TC_CASE(ex_next_char)   { double d = 7.0; char c = 0; int r; in = "1ex"; r = sscanf(in, "%lf%c", &d, &c); CHECK_EQ_I(r, 2); CHECK(c == 'x'); }

#endif
int main(void) { tc_begin(); TC_RUN(e_only_value); TC_RUN(e_only_n); TC_RUN(e_plus_value); TC_RUN(e_plus_n); TC_RUN(ex_value); TC_RUN(ex_next_char); return tc_end(); }
