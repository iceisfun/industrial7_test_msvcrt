/* TC
 * id:    scn_neg_sscanf_octal_89_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.21.6.2p12 (o matches octal digits only)
 * note:  PROBES that %o rejects 8/9: "89" -> ret 0, "18" -> ret 1 value 1 stopping at 8. Standard says so; older MS CRTs may accept any decimal digit.
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile in;
static const char *volatile fmt;

TC_CASE(eight_nine_octal) { unsigned v = 77; int r; in = "89"; fmt = "%o"; r = sscanf(in, fmt, &v); CHECK_EQ_I(r, 0); CHECK_EQ_U(v, 77); }
TC_CASE(nine_octal)       { unsigned v = 77; int r, n = -1; in = "9"; fmt = "%o%n"; r = sscanf(in, fmt, &v, &n); CHECK_EQ_I(r, 0); CHECK_EQ_U(v, 77); CHECK_EQ_I(n, -1); }
TC_CASE(partial_octal)    { unsigned v = 77; int r, n = -1; in = "18"; fmt = "%o%n"; r = sscanf(in, fmt, &v, &n); CHECK_EQ_I(r, 1); CHECK_EQ_U(v, 1); CHECK_EQ_I(n, 1); }

int main(void) { tc_begin(); TC_RUN(eight_nine_octal); TC_RUN(nine_octal); TC_RUN(partial_octal); return tc_end(); }
