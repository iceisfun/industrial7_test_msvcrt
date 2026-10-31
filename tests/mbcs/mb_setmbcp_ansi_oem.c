/* TC
 * id:    mb_setmbcp_ansi_oem
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _setmbcp (_MB_CP_ANSI, _MB_CP_OEM)
 * note:  ACP/OEMCP of the test machine is arbitrary; getmbcp accepted as 0 (SBCS) or the kernel32 value. Assumes ACP/OEM are not 65001.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <errno.h>

static volatile int v_cp;
/* _getmbcp returns 0 for single-byte code pages (docs), so on SBCS machines accept 0 or the cp */
static void chk(int mode, int want)
{
    int s = _getmbcp(), g;
    v_cp = mode;
    CHECK_EQ_I(_setmbcp(v_cp), 0);
    g = _getmbcp();
    CHECK(g == 0 || g == want);
    _setmbcp(s);
}
TC_CASE(ansi) { chk(_MB_CP_ANSI, (int)GetACP()); }
TC_CASE(oem)  { chk(_MB_CP_OEM, (int)GetOEMCP()); }

int main(void) { tc_begin(); TC_RUN(ansi); TC_RUN(oem); return tc_end(); }
