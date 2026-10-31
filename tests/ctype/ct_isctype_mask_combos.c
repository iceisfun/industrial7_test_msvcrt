/* TC
 * id:    ct_isctype_mask_combos
 * area:  ctype
 * funcs: _isctype
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _isctype (nonzero if c has any of the property bits in mask)
 * note:  tab is deliberately not tested with _BLANK: the table bit for tab is _SPACE|_CONTROL on Windows, isblank special-cases it
 */
#include "tc.h"
#include <ctype.h>

static volatile int vA = 'A', va = 'a', v5 = '5', vsp = ' ', vx = '#';

TC_CASE(upper_or_lower_true_for_a) { CHECK(_isctype(va, _UPPER | _LOWER)); CHECK(_isctype(vA, _UPPER | _LOWER)); }
TC_CASE(digit_or_alpha_true_for_5) { CHECK(_isctype(v5, _DIGIT | _ALPHA)); CHECK(_isctype(va, _DIGIT | _ALPHA)); CHECK(!_isctype(vx, _DIGIT | _ALPHA)); }
TC_CASE(upper_or_lower_false_for_5) { CHECK(!_isctype(v5, _UPPER | _LOWER)); CHECK(!_isctype(vsp, _UPPER | _LOWER)); }
TC_CASE(blank_true_for_space) { CHECK(_isctype(vsp, _BLANK)); CHECK(!_isctype(vA, _BLANK)); }

int main(void) { tc_begin(); TC_RUN(upper_or_lower_true_for_a); TC_RUN(digit_or_alpha_true_for_5); TC_RUN(upper_or_lower_false_for_5); TC_RUN(blank_true_for_space); return tc_end(); }
