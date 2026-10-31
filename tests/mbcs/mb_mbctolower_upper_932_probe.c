/* TC
 * id:    mb_mbctolower_upper_932_probe
 * area:  mbcs
 * funcs: _mbctolower _mbctoupper _setmbcp _getmbcp
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _mbctolower, _mbctoupper; cp932 full-width Latin
 * note:  PROBE: under cp932 full-width Latin letters are case-mapped (0x8260 <-> 0x8281) and a kanji (0x889F) is unchanged.
 */
#include "tc.h"
#include <mbstring.h>
static int old_cp;
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(fullwidth)
{
    CHECK_EQ_U(_mbctolower(0x8260), 0x8281);
    CHECK_EQ_U(_mbctoupper(0x8281), 0x8260);
}
TC_CASE(kanji) { CHECK_EQ_U(_mbctolower(0x889F), 0x889F); CHECK_EQ_U(_mbctoupper(0x889F), 0x889F); }
int main(void) { tc_begin(); old_cp = _getmbcp(); TC_RUN(cp); TC_RUN(fullwidth); TC_RUN(kanji); _setmbcp(old_cp); return tc_end(); }
