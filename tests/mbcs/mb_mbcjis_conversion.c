/* TC
 * id:    mb_mbcjis_conversion
 * area:  mbcs
 * funcs: _mbcjmstojis _mbcjistojms _setmbcp
 * crt:   all
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _mbcjmstojis, _mbcjistojms (code page 932 only)
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;

TC_CASE(to_jis) { CHECK_EQ_U(_setmbcp(932), 0); v = 0x889F; CHECK_EQ_U(_mbcjmstojis(v), 0x3021); v = 0x8140; CHECK_EQ_U(_mbcjmstojis(v), 0x2121); }
TC_CASE(to_jms) { CHECK_EQ_U(_setmbcp(932), 0); v = 0x3021; CHECK_EQ_U(_mbcjistojms(v), 0x889F); v = 0x2121; CHECK_EQ_U(_mbcjistojms(v), 0x8140); }
TC_CASE(roundtrip) { unsigned int c; CHECK_EQ_U(_setmbcp(932), 0); for (c = 0x8140; c <= 0x8150; c++) { v = c; if (c == 0x817F) continue; CHECK_EQ_U(_mbcjistojms(_mbcjmstojis(v)), c); } }

int main(void) { tc_begin(); TC_RUN(to_jis); TC_RUN(to_jms); TC_RUN(roundtrip); _setmbcp(-3); return tc_end(); }
