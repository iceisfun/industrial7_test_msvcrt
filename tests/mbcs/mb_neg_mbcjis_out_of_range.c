/* TC
 * id:    mb_neg_mbcjis_out_of_range
 * area:  mbcs
 * funcs: _mbcjmstojis _mbcjistojms _setmbcp
 * kind:  negative
 * crt:   all
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _mbcjmstojis, _mbcjistojms (return 0 for illegal characters)
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;

TC_CASE(jms_illegal) { CHECK_EQ_U(_setmbcp(932), 0); v = 0x41; CHECK_EQ_U(_mbcjmstojis(v), 0); v = 0xFF40; CHECK_EQ_U(_mbcjmstojis(v), 0); }
TC_CASE(jis_illegal) { CHECK_EQ_U(_setmbcp(932), 0); v = 0x0000; CHECK_EQ_U(_mbcjistojms(v), 0); v = 0x2020; CHECK_EQ_U(_mbcjistojms(v), 0); }

int main(void) { tc_begin(); TC_RUN(jms_illegal); TC_RUN(jis_illegal); _setmbcp(-3); return tc_end(); }
