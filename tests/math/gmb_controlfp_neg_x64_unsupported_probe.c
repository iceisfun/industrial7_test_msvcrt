/* TC
 * id:    gmb_controlfp_neg_x64_unsupported_probe
 * area:  math
 * funcs: _controlfp_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _controlfp_s (platform notes: _MCW_PC / _MCW_IC on x64/ARM)
 * note:  PROBE: x64 handling of _PC_24/_MCW_PC and _IC_AFFINE/_MCW_IC (ignored -> 0, or rejected -> EINVAL) and that
 *        _DN_FLUSH/_MCW_DN works. Accepts either 0 or EINVAL for PC/IC; always restores the control word.
 */
#include "tc.h"
#include <float.h>
#include <errno.h>

static unsigned int rd(void) { unsigned int cw = 0; _controlfp_s(&cw, 0, 0); return cw; }

TC_CASE(pc_bits)
{
    unsigned int orig = rd(), cw = 0; int r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    r = _controlfp_s(&cw, _PC_24, _MCW_PC);
    CHECK(r == 0 || r == EINVAL);
    _controlfp_s(&cw, orig & (_MCW_PC | _MCW_RC | _MCW_EM), _MCW_PC | _MCW_RC | _MCW_EM);
    CHECK_EQ_U(rd() & (_MCW_RC | _MCW_EM), orig & (_MCW_RC | _MCW_EM));
}
TC_CASE(ic_bits)
{
    unsigned int orig = rd(), cw = 0; int r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    r = _controlfp_s(&cw, _IC_AFFINE, _MCW_IC);
    CHECK(r == 0 || r == EINVAL);
    _controlfp_s(&cw, orig & _MCW_IC, _MCW_IC);
    CHECK_EQ_U(rd() & (_MCW_RC | _MCW_EM), orig & (_MCW_RC | _MCW_EM));
}
TC_CASE(dn_flush)
{
    unsigned int orig = rd(), cw = 0; int r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    r = _controlfp_s(&cw, _DN_FLUSH, _MCW_DN);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_U(rd() & _MCW_DN, _DN_FLUSH);
    _controlfp_s(&cw, orig & _MCW_DN, _MCW_DN);
    CHECK_EQ_U(rd() & _MCW_DN, orig & _MCW_DN);
}

int main(void)
{
    tc_begin();
    TC_RUN(pc_bits); TC_RUN(ic_bits); TC_RUN(dn_flush);
    return tc_end();
}
