/* TC
 * id:    ct_neg_isleadbyte_out_of_range
 * area:  ctype
 * funcs: isleadbyte
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs isleadbyte
 * note:  PROBE: isleadbyte with ints outside 0..255 in the C locale; expected 0 (old CRTs truncate to unsigned char, ucrt range-checks). Invalid-parameter handler installed in case ucrt reports; its count is not asserted.
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;

TC_CASE(c256_returns_0) { vc = 256; CHECK_EQ_I(isleadbyte(vc), 0); }
TC_CASE(c0x181_returns_0) { vc = 0x181; CHECK_EQ_I(isleadbyte(vc), 0); }
TC_CASE(cminus1_returns_0) { vc = -1; CHECK_EQ_I(isleadbyte(vc), 0); }
TC_CASE(cminus2_returns_0) { vc = -2; CHECK_EQ_I(isleadbyte(vc), 0); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(c256_returns_0); TC_RUN(c0x181_returns_0); TC_RUN(cminus1_returns_0); TC_RUN(cminus2_returns_0); return tc_end(); }
