/* TC
 * id:    mb_neg_probe_setmbcp_failure_state
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _setmbcp
 * note:  probe: a failed _setmbcp(12345) leaves the previous code page (932) in effect; undocumented.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile int v_cp;
TC_CASE(state_kept)
{
    int s = _getmbcp();
    CHECK_EQ_I(_setmbcp(932), 0);
    v_cp = 12345;
    CHECK_EQ_I(_setmbcp(v_cp), -1);
    CHECK_EQ_I(_getmbcp(), 932);
    _setmbcp(s);
}

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(state_kept); return tc_end(); }
