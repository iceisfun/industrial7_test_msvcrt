/* TC
 * id:    mb_neg_setmbcp_boundary_codes
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _setmbcp: returns -1 on error; valid specials are _MB_CP_SBCS(0) _MB_CP_OEM(-2) _MB_CP_ANSI(-3) _MB_CP_LOCALE(-4)
 * note:  -1, -5 and INT_MAX are not valid code pages and must return -1. State preservation after failure is NOT asserted
 *        here (undocumented; see mb_neg_probe_setmbcp_failure_state). INT_MIN and 65536 are in
 *        mb_neg_probe_setmbcp_special_negatives because the CRT may narrow the value to 16 bits.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <limits.h>

static int old_cp;
static volatile int v_cp;

static void fails(int cp)
{
    _setmbcp(932);
    v_cp = cp;
    CHECK_EQ_I(_setmbcp(v_cp), -1);
}
TC_CASE(minus1_fails) { fails(-1); }
TC_CASE(minus5_fails) { fails(-5); }
TC_CASE(int_max_fails) { fails(INT_MAX); }
int main(void)
{
    tc_begin(); tc_iph_install(); old_cp = _getmbcp();
    TC_RUN(minus1_fails); TC_RUN(minus5_fails); TC_RUN(int_max_fails);
    _setmbcp(old_cp);
    return tc_end();
}
