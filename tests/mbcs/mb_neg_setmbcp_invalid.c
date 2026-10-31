/* TC
 * id:    mb_neg_setmbcp_invalid
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _setmbcp: returns -1 if an error occurs
 * note:  errno not asserted here; "state unchanged after failure" is not documented (see mb_neg_probe_setmbcp_failure_state)
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <errno.h>

static volatile int v_cp;
static void fails(int cp)
{
    v_cp = cp;
    CHECK_EQ_I(_setmbcp(v_cp), -1);
}
TC_CASE(cp12345) { int s = _getmbcp(); fails(12345); _setmbcp(s); }
TC_CASE(neg100)  { int s = _getmbcp(); fails(-100); _setmbcp(s); }
TC_CASE(cp1)     { int s = _getmbcp(); fails(1); _setmbcp(s); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(cp12345); TC_RUN(neg100); TC_RUN(cp1); return tc_end(); }
