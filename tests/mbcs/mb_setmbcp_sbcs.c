/* TC
 * id:    mb_setmbcp_sbcs
 * area:  mbcs
 * funcs: _setmbcp _getmbcp _ismbblead _mbstrlen
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _setmbcp (_MB_CP_SBCS = 0)
 * note:  Spec said -3 but _MB_CP_SBCS is 0 in mbctype.h (-3 is _MB_CP_ANSI).
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <errno.h>

static volatile int v_cp;
TC_CASE(sbcs)
{
    static const char s[] = "\x81\x40\xE0\xA0";
    int saved = _getmbcp();
    v_cp = _MB_CP_SBCS;
    CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK_EQ_I(_getmbcp(), 0);
    CHECK_EQ_I(_ismbblead(0x81), 0);
    CHECK_EQ_I(_ismbblead(0xE0), 0);
    CHECK_EQ_U(_mbstrlen(s), 4);
    _setmbcp(saved);
}

int main(void) { tc_begin(); TC_RUN(sbcs); return tc_end(); }
