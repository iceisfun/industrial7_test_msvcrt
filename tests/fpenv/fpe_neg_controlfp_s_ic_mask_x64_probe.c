/* TC
 * id:    fpe_neg_controlfp_s_ic_mask_x64_probe
 * area:  fpenv
 * funcs: _controlfp_s
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _controlfp_s: _MCW_IC not supported on x64
 * note:  Probes what _controlfp_s(&c,_IC_AFFINE,_MCW_IC) returns on x64 (0 or EINVAL, internally consistent with the invalid-parameter handler) and that the IC field is never toggled. Only consistency is asserted, not the specific outcome.
 */
#include "tc.h"
#include <float.h>
#include <errno.h>

TC_CASE(ic_request)
{
    unsigned before = 0, c = 0xdeadbeef, after = 0; int r, base;
    if (!TC_X64) TC_SKIP("x64 only");
    if (!tc_iph_install()) TC_SKIP("no iph api");
    CHECK_EQ_I(_controlfp_s(&before, 0, 0), 0);
    base = tc_iph_count;
    r = _controlfp_s(&c, _IC_AFFINE, _MCW_IC);
    CHECK(r == 0 || r == EINVAL);
    if (r == 0) CHECK_EQ_I(tc_iph_count, base); else CHECK_EQ_I(tc_iph_count, base + 1);
    CHECK_EQ_I(_controlfp_s(&after, 0, 0), 0);
    CHECK_EQ_U(after & _MCW_IC, before & _MCW_IC);
    CHECK_EQ_U(after & (_MCW_RC | _MCW_EM), before & (_MCW_RC | _MCW_EM));
}
int main(void) { tc_begin(); TC_RUN(ic_request); return tc_end(); }
