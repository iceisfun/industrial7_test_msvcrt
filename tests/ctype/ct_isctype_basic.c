/* TC
 * id:    ct_isctype_basic
 * area:  ctype
 * funcs: _isctype
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _isctype; ctype.h mask constants
 */
#include "tc.h"
#include <ctype.h>

static volatile int vA = 'A', va = 'a', v5 = '5', vsp = ' ', vpu = '!', vnl = '\n', vf = 'f', vg = 'g';

TC_CASE(upper_mask) { CHECK(_isctype(vA, _UPPER)); CHECK(!_isctype(va, _UPPER)); CHECK(!_isctype(v5, _UPPER)); }
TC_CASE(lower_mask) { CHECK(_isctype(va, _LOWER)); CHECK(!_isctype(vA, _LOWER)); CHECK(!_isctype(v5, _LOWER)); }
TC_CASE(digit_mask) { CHECK(_isctype(v5, _DIGIT)); CHECK(!_isctype(vA, _DIGIT)); CHECK(!_isctype(vsp, _DIGIT)); }
TC_CASE(alpha_mask_0x103)
{
    CHECK_EQ_I(_ALPHA, 0x103);
    CHECK(_isctype(vA, 0x103)); CHECK(_isctype(va, 0x103));
    CHECK(!_isctype(v5, 0x103)); CHECK(!_isctype(vsp, 0x103));
}
TC_CASE(space_mask) { CHECK(_isctype(vsp, _SPACE)); CHECK(_isctype(vnl, _SPACE)); CHECK(!_isctype(vA, _SPACE)); }
TC_CASE(punct_mask) { CHECK(_isctype(vpu, _PUNCT)); CHECK(!_isctype(vA, _PUNCT)); CHECK(!_isctype(v5, _PUNCT)); CHECK(!_isctype(vsp, _PUNCT)); }
TC_CASE(control_mask) { CHECK(_isctype(vnl, _CONTROL)); CHECK(_isctype(0x7f, _CONTROL)); CHECK(!_isctype(vA, _CONTROL)); CHECK(!_isctype(vsp, _CONTROL)); }
TC_CASE(hex_mask) { CHECK(_isctype(vf, _HEX)); CHECK(_isctype(vA, _HEX)); CHECK(_isctype(v5, _HEX)); CHECK(!_isctype(vg, _HEX)); }

int main(void) { tc_begin(); TC_RUN(upper_mask); TC_RUN(lower_mask); TC_RUN(digit_mask); TC_RUN(alpha_mask_0x103); TC_RUN(space_mask); TC_RUN(punct_mask); TC_RUN(control_mask); TC_RUN(hex_mask); return tc_end(); }
