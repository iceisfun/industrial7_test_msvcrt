/* TC
 * id:    scn_neg_sscanf_set_nomatch_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.21.6.2p12 ([ conversion), p16
 * note:  Probes that a %[ conversion that matches zero characters returns 0 and leaves the destination buffer completely
 *        untouched (no terminating NUL written). C implies no assignment on matching failure; MSVC behaviour is measured here.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(nomatch_buffer_untouched) { char s[4] = {'a','b','c',0}; int r = sscanf("123", "%[a-z]", s); CHECK_EQ_I(r, 0); CHECK_STR(s, "abc"); }
TC_CASE(leading_ws_nomatch_buffer_untouched) { char s[4] = {'a','b','c',0}; int r = sscanf(" ab", "%[a-z]", s); CHECK_EQ_I(r, 0); CHECK_STR(s, "abc"); }

int main(void) { tc_begin(); TC_RUN(nomatch_buffer_untouched); TC_RUN(leading_ws_nomatch_buffer_untouched); return tc_end(); }
