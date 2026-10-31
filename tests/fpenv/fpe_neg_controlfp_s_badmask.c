/* TC
 * id:    fpe_neg_controlfp_s_badmask
 * area:  fpenv
 * funcs: 
 * procs: _controlfp_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _controlfp_s (EINVAL and invalid parameter handler for invalid mask/value bits)
 * note:  PROBE: _controlfp_s(&cur, ~0, ~0) contains bits that are not valid control-word bits; expect EINVAL and the invalid-parameter handler, with the control word left unchanged. Exact validity rule is not documented in detail.
 */
#include "tc.h"
#include <float.h>
#include <errno.h>

typedef int (__cdecl *cfs_t)(unsigned int *, unsigned int, unsigned int);

TC_CASE(bad_bits_einval)
{
    cfs_t p = (cfs_t)tc_crt_proc("_controlfp_s");
    unsigned int cur = 0x12345678u, before = _controlfp(0, 0);
    int r;
    if (!p) TC_SKIP("no _controlfp_s");
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0;
    r = p(&cur, 0xFFFFFFFFu, 0xFFFFFFFFu);
    CHECK_EQ_I(r, EINVAL);
    CHECK(tc_iph_count >= 1);
    CHECK_EQ_U(_controlfp(0, 0), before);
    _controlfp(before, _MCW_RC | _MCW_EM);
}
int main(void) { tc_begin(); TC_RUN(bad_bits_einval); return tc_end(); }
