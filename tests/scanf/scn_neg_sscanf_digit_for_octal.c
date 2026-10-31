/* TC
 * id:    scn_neg_sscanf_digit_for_octal
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.21.6.2p12 (x conversion matches hex digits only), p16 (matching failure returns count so far)
 * note:  A leading character that is not a hex digit is a matching failure: return 0, nothing stored, scanning stops.
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile in;
static const char *volatile fmt;

TC_CASE(g_hex)            { unsigned v = 77; int r; in = "g"; fmt = "%x"; r = sscanf(in, fmt, &v); CHECK_EQ_I(r, 0); CHECK_EQ_U(v, 77); }
TC_CASE(later_not_run)    { unsigned v = 77; char c = 'Z'; int r; in = "g1"; fmt = "%x%c"; r = sscanf(in, fmt, &v, &c); CHECK_EQ_I(r, 0); CHECK_EQ_U(v, 77); CHECK(c == 'Z'); }

int main(void) { tc_begin(); TC_RUN(g_hex); TC_RUN(later_not_run); return tc_end(); }
