/* TC
 * id:    scn_neg_sscanf_scanset_invalid_range_probe
 * area:  scanf
 * kind:  negative
 * funcs: sscanf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   C11 7.21.6.2p12 (range in scanset is implementation-defined); MS docs scanf type field characters
 * note:  Probes MS scanset handling. Believed: a descending range '[z-a]' is not a range (z, '-', a become literals, so 'mmm' does not match -> 0); a trailing '-' is a literal ('[a-]' matches "a-"); '[^]' has ']' as first char so the set is unterminated -> ret <= 0 and no crash; '[a-z-9]' is a-z, then literal '-' and '9'.
 *        Win10 22H2 ucrtbase (observed 2026-10-31, stable over 2 runs): a descending range [z-a] is accepted as the ordinary range a..z ("mmm" matches, "5mm" and "A" do not). C11 7.21.6.2p12 leaves this implementation-defined; Windows normalises the range.
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *vin;

TC_CASE(descending_z_a) { char b[16] = "Q"; int r; vin = "mmm"; r = sscanf((const char *)vin, "%[z-a]", b); CHECK_EQ_I(r, 1); CHECK_STR(b, "mmm");
    vin = "5mm"; r = sscanf((const char *)vin, "%[z-a]", b); CHECK_EQ_I(r, 0); }
TC_CASE(trailing_dash) { char b[16] = "Q"; int r; vin = "a-b"; r = sscanf((const char *)vin, "%[a-]", b); CHECK_EQ_I(r, 1); CHECK_STR(b, "a-"); }
TC_CASE(caret_close_bracket_unterminated) { char b[16] = "Q"; int r; tc_iph_install(); vin = "abc"; r = sscanf((const char *)vin, "%[^]", b); CHECK(r <= 0); CHECK_EQ_I(b[15], 0); }
TC_CASE(range_then_dash_digit) { char b[16] = "Q"; int r; vin = "ab-9!"; r = sscanf((const char *)vin, "%[a-z-9]", b); CHECK_EQ_I(r, 1); CHECK_STR(b, "ab-9"); }

int main(void) { tc_begin(); TC_RUN(descending_z_a); TC_RUN(trailing_dash); TC_RUN(caret_close_bracket_unterminated); TC_RUN(range_then_dash_digit); return tc_end(); }
