/* TC
 * id:    mb_neg_ismbc_outside_932
 * area:  mbcs
 * funcs: _ismbchira _ismbckata _ismbcl1 _ismbcl2 _setmbcp
 * kind:  negative
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _ismbchira etc. (Japanese-specific)
 * note:  PROBE: JIS-specific predicates return 0 when the multibyte code page is not 932 (936 and 1252).
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;

TC_CASE(cp936) { CHECK_EQ_U(_setmbcp(936), 0); v = 0x829F; CHECK_EQ_I(_ismbchira(v), 0); v = 0x8340; CHECK_EQ_I(_ismbckata(v), 0); v = 0x889F; CHECK_EQ_I(_ismbcl1(v), 0); v = 0x989F; CHECK_EQ_I(_ismbcl2(v), 0); }
TC_CASE(cp1252) { CHECK_EQ_U(_setmbcp(1252), 0); v = 0x829F; CHECK_EQ_I(_ismbchira(v), 0); v = 0x8340; CHECK_EQ_I(_ismbckata(v), 0); v = 0x889F; CHECK_EQ_I(_ismbcl1(v), 0); v = 0x989F; CHECK_EQ_I(_ismbcl2(v), 0); }

int main(void) { tc_begin(); TC_RUN(cp936); TC_RUN(cp1252); _setmbcp(-3); return tc_end(); }
