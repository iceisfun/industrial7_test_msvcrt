/* TC
 * id:    gmb_controlfp_neg_set_invalid_mask
 * area:  math
 * funcs: _controlfp_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _controlfp_s: invalid mask/new bits -> EINVAL and the invalid parameter handler is invoked
 * note:  Uses bit 0x400 (outside _MCW_EM|_MCW_RC|_MCW_PC|_MCW_IC|_MCW_DN) in both newControl and mask.
 */
#include "tc.h"
#include <float.h>
#include <errno.h>

static volatile unsigned int v_mask;

TC_CASE(unknown_bit_rejected)
{
    unsigned int before = 0, cw = 0x12345678, after = 0; int r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    _controlfp_s(&before, 0, 0);
    tc_iph_count = 0;
    v_mask = 0x400;
    r = _controlfp_s(&cw, v_mask, v_mask);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    _controlfp_s(&after, 0, 0);
    CHECK_EQ_U(after, before);
}
TC_CASE(all_ones_mask_rejected)
{
    unsigned int before = 0, cw = 0, after = 0; int r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    _controlfp_s(&before, 0, 0);
    tc_iph_count = 0;
    v_mask = 0xFFFFFFFFu;
    r = _controlfp_s(&cw, v_mask, v_mask);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    _controlfp_s(&after, 0, 0);
    CHECK_EQ_U(after, before);
}
TC_CASE(valid_still_ok)
{
    unsigned int cw = 0; int r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0;
    r = _controlfp_s(&cw, 0, 0);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin();
    TC_RUN(unknown_bit_rejected); TC_RUN(all_ones_mask_rejected); TC_RUN(valid_still_ok);
    return tc_end();
}
