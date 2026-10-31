/* TC
 * id:    mb_neg_probe_setmbcp_unsupported_cp
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _setmbcp
 * note:  probe: _setmbcp(65001) and _setmbcp(1200) expected -1 (max char len >2 / not a valid ANSI cp). Real Windows is the oracle.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <errno.h>

static volatile int v_cp;
TC_CASE(utf8) { int s = _getmbcp(); v_cp = 65001; CHECK_EQ_I(_setmbcp(v_cp), -1); _setmbcp(s); }
TC_CASE(utf16) { int s = _getmbcp(); v_cp = 1200; CHECK_EQ_I(_setmbcp(v_cp), -1); _setmbcp(s); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(utf8); TC_RUN(utf16); return tc_end(); }
