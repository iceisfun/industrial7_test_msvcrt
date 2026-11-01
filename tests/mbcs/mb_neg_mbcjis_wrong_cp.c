/* TC
 * id:    mb_neg_mbcjis_wrong_cp
 * area:  mbcs
 * funcs: _mbcjmstojis _mbcjistojms _setmbcp
 * kind:  negative
 * crt:   all
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _mbcjmstojis (requires code page 932)
 * note:  PROBE: conversions return 0 when the multibyte code page is 936 or 1252.
 *        Observed Win10 22H2 ucrtbase: with MBCS code page 936 or 1252 the conversions do not convert: they return the argument
 *        unchanged (0x889F -> 0x889F, 0x3021 -> 0x3021), not 0; with CP932 they convert (0x889F <-> 0x3021). Stable.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;

TC_CASE(cp936) { CHECK_EQ_U(_setmbcp(936), 0); v = 0x889F; CHECK_EQ_U(_mbcjmstojis(v), 0x889F); v = 0x3021; CHECK_EQ_U(_mbcjistojms(v), 0x3021); }   /* observed: unchanged, not 0 */
TC_CASE(cp1252) { CHECK_EQ_U(_setmbcp(1252), 0); v = 0x889F; CHECK_EQ_U(_mbcjmstojis(v), 0x889F); v = 0x3021; CHECK_EQ_U(_mbcjistojms(v), 0x3021); }   /* observed: unchanged, not 0 */

int main(void) { tc_begin(); TC_RUN(cp936); TC_RUN(cp1252); _setmbcp(-3); return tc_end(); }
