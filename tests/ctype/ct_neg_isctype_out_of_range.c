/* TC
 * id:    ct_neg_isctype_out_of_range
 * area:  ctype
 * funcs: _isctype
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _isctype
 * note:  PROBE: _isctype with c outside 0..255 in the C locale; expected 0 (old CRTs classify the low byte via GetStringTypeA: NUL / '4' are not alpha). -2 deliberately omitted: its low byte 0xFE is code-page dependent. Invalid-parameter handler installed in case ucrt reports; count not asserted. EOF case expected 0 (documented valid input -1).
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;

TC_CASE(isctype_256_ALPHA_zero) { vc = 256; CHECK_EQ_I(_isctype(vc, _ALPHA), 0); }
TC_CASE(isctype_0x1234_ALPHA_zero) { vc = 0x1234; CHECK_EQ_I(_isctype(vc, _ALPHA), 0); }
TC_CASE(isctype_EOF_any_zero) { vc = -1; CHECK_EQ_I(_isctype(vc, _ALPHA), 0); CHECK_EQ_I(_isctype(vc, _DIGIT), 0); CHECK_EQ_I(_isctype(vc, _SPACE), 0); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(isctype_256_ALPHA_zero); TC_RUN(isctype_0x1234_ALPHA_zero); TC_RUN(isctype_EOF_any_zero); return tc_end(); }
