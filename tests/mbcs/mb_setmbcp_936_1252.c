/* TC
 * id:    mb_setmbcp_936_1252
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _setmbcp
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <errno.h>

static volatile int v_cp;
static int set(int cp) { v_cp = cp; return _setmbcp(v_cp); }
TC_CASE(cp936) { int s = _getmbcp(); CHECK_EQ_I(set(936), 0); CHECK_EQ_I(_getmbcp(), 936); _setmbcp(s); }
/* docs: _getmbcp returns 0 for a single-byte code page; whether 1252 reads back as 1252 or 0 is probed separately */
TC_CASE(cp1252) { int s = _getmbcp(), g; CHECK_EQ_I(set(1252), 0); g = _getmbcp(); CHECK(g == 0 || g == 1252); _setmbcp(s); }
TC_CASE(sequence)
{
    int s = _getmbcp();
    CHECK_EQ_I(set(932), 0); CHECK_EQ_I(_getmbcp(), 932);
    CHECK_EQ_I(set(1252), 0);
    CHECK_EQ_I(set(936), 0); CHECK_EQ_I(_getmbcp(), 936);
    _setmbcp(s);
}

int main(void) { tc_begin(); TC_RUN(cp936); TC_RUN(cp1252); TC_RUN(sequence); return tc_end(); }
