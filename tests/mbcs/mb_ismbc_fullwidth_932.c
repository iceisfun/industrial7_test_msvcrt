/* TC
 * id:    mb_ismbc_fullwidth_932
 * area:  mbcs
 * funcs: _ismbcalpha _ismbcdigit _ismbcspace _ismbcupper _ismbclower _setmbcp
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _ismbcalpha etc.
 * note:  PROBE: whether double-byte full-width characters (hiragana 0x82A0, fullwidth digit 0x824F, ideographic space 0x8140, fullwidth A 0x8260, fullwidth a 0x8281) are classified alpha/digit/space/upper/lower under cp932 (Win32 C1 info).
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;

TC_CASE(alpha) { CHECK_EQ_U(_setmbcp(932), 0); v = 0x82A0; CHECK(_ismbcalpha(v)); v = 0x8260; CHECK(_ismbcalpha(v)); }
TC_CASE(digit) { CHECK_EQ_U(_setmbcp(932), 0); v = 0x824F; CHECK(_ismbcdigit(v)); }
TC_CASE(space) { CHECK_EQ_U(_setmbcp(932), 0); v = 0x8140; CHECK(_ismbcspace(v)); }
TC_CASE(case_) { CHECK_EQ_U(_setmbcp(932), 0); v = 0x8260; CHECK(_ismbcupper(v)); v = 0x8281; CHECK(_ismbclower(v)); }

int main(void) { tc_begin(); TC_RUN(alpha); TC_RUN(digit); TC_RUN(space); TC_RUN(case_); _setmbcp(-3); return tc_end(); }
