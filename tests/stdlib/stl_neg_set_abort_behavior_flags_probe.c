/* TC
 * id:    stl_neg_set_abort_behavior_flags_probe
 * area:  stdlib
 * kind:  negative
 * funcs: _set_abort_behavior
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _set_abort_behavior (returns previous flags; only bits in mask change)
 * note:  Mask 0 and flags-outside-mask leave state unchanged (documented semantics: new = (old & ~mask) | (flags & mask)).
 *        PROBES unknown bits: expectation is that the CRT does NOT filter them (0x80000000 in mask is stored and returned later).
 *        Original flags are restored at the end.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static unsigned int orig;

TC_CASE(mask_zero_changes_nothing)
{
    unsigned int a, b; errno = 0; tc_iph_count = 0;
    a = _set_abort_behavior(0, 0); b = _set_abort_behavior(0, 0);
    CHECK_EQ_U(a, b); CHECK_EQ_U(a, orig); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(flags_outside_mask_ignored)
{
    unsigned int before, after;
    before = _set_abort_behavior(0, 0);
    _set_abort_behavior(0x3, 0);
    after = _set_abort_behavior(0, 0);
    CHECK_EQ_U(before, after);
}
TC_CASE(set_and_clear_known_bit)
{
    unsigned int prev, now;
    _set_abort_behavior(0, 0x3);
    prev = _set_abort_behavior(0x2, 0x2);
    CHECK_EQ_U(prev & 0x3, 0);
    now = _set_abort_behavior(0, 0);
    CHECK_EQ_U(now & 0x3, 0x2);
    _set_abort_behavior(0, 0x2);
    CHECK_EQ_U(_set_abort_behavior(0, 0) & 0x3, 0);
}
TC_CASE(unknown_high_bits_probe)
{
    unsigned int now;
    _set_abort_behavior(0x80000000u, 0x80000000u);
    now = _set_abort_behavior(0, 0);
    CHECK_EQ_U(now & 0x80000000u, 0x80000000u);
    _set_abort_behavior(0, 0x80000000u);
    CHECK_EQ_U(_set_abort_behavior(0, 0) & 0x80000000u, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    orig = _set_abort_behavior(0, 0);
    TC_RUN(mask_zero_changes_nothing); TC_RUN(flags_outside_mask_ignored); TC_RUN(set_and_clear_known_bit); TC_RUN(unknown_high_bits_probe);
    _set_abort_behavior(orig, 0xFFFFFFFFu);
    return tc_end();
}
