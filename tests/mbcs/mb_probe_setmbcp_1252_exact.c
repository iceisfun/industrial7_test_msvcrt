/* TC
 * id:    mb_probe_setmbcp_1252_exact
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _getmbcp (0 means single-byte code page)
 * note:  probe: after _setmbcp(1252) does _getmbcp() read back 1252 (assumed) or 0? Real Windows is the oracle.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile int v_cp = 1252;
TC_CASE(readback)
{
    int s = _getmbcp();
    CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK_EQ_I(_getmbcp(), 1252);
    _setmbcp(s);
}

int main(void) { tc_begin(); TC_RUN(readback); return tc_end(); }
