/* TC
 * id:    mb_ismbc_kana_kanji_932
 * area:  mbcs
 * funcs: _ismbchira _ismbckata _ismbcl0 _ismbcl1 _ismbcl2 _setmbcp
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _ismbchira, _ismbckata, _ismbcl0, _ismbcl1, _ismbcl2
 * note:  Documented ranges: hira 0x829F-0x82F1, kata 0x8340-0x8396, L0 0x8140-0x889E, L1 0x889F-0x9872, L2 0x989F-0xEAA4 (cp932 only).
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;

TC_CASE(hira) { CHECK_EQ_U(_setmbcp(932), 0); v = 0x829F; CHECK(_ismbchira(v)); v = 0x82F1; CHECK(_ismbchira(v)); v = 0x8340; CHECK(!_ismbchira(v)); }
TC_CASE(kata) { CHECK_EQ_U(_setmbcp(932), 0); v = 0x8340; CHECK(_ismbckata(v)); v = 0x8396; CHECK(_ismbckata(v)); v = 0x829F; CHECK(!_ismbckata(v)); }
TC_CASE(l0) { CHECK_EQ_U(_setmbcp(932), 0); v = 0x8140; CHECK(_ismbcl0(v)); v = 0x84BE; CHECK(_ismbcl0(v)); v = 0x889F; CHECK(!_ismbcl0(v)); }
TC_CASE(l1) { CHECK_EQ_U(_setmbcp(932), 0); v = 0x889F; CHECK(_ismbcl1(v)); v = 0x9872; CHECK(_ismbcl1(v)); v = 0x8140; CHECK(!_ismbcl1(v)); }
TC_CASE(l2) { CHECK_EQ_U(_setmbcp(932), 0); v = 0x989F; CHECK(_ismbcl2(v)); v = 0xEAA4; CHECK(_ismbcl2(v)); v = 0x889F; CHECK(!_ismbcl2(v)); }

int main(void) { tc_begin(); TC_RUN(hira); TC_RUN(kata); TC_RUN(l0); TC_RUN(l1); TC_RUN(l2); _setmbcp(-3); return tc_end(); }
