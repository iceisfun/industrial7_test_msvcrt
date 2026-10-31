/* TC
 * id:    fpe_neg_control87_unknown_mask_bits_ignored
 * area:  fpenv
 * kind:  negative
 * funcs: _control87 _controlfp
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _control87/_controlfp (no error return; only _MCW_* fields are documented)
 * note:  PROBING: mask bits outside any _MCW_* field (0x00400000, 0x00200000) neither call the invalid-parameter handler nor change
 *        RC/EM/DN, and the legacy functions never return 0xFFFFFFFF. Only mask bits are probed with newControl == 0 so a
 *        hypothetical interpretation of the bits could at worst clear them.
 */
#include "tc.h"
#include <float.h>
#define KNOWN (_MCW_EM | _MCW_RC | _MCW_DN)

TC_CASE(control87_unknown)
{
    unsigned int before, r, after; volatile unsigned int m = 0x00400000u | 0x00200000u;
    tc_iph_install(); tc_iph_count = 0;
    before = _control87(0, 0);
    r = _control87(0, m);
    after = _control87(0, 0);
    CHECK(r != 0xFFFFFFFFu);
    CHECK_EQ_U(after & KNOWN, before & KNOWN);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(controlfp_unknown)
{
    unsigned int before, r, after; volatile unsigned int m = 0x00400000u | 0x00200000u;
    tc_iph_install(); tc_iph_count = 0;
    before = _controlfp(0, 0);
    r = _controlfp(0, m);
    after = _controlfp(0, 0);
    CHECK(r != 0xFFFFFFFFu);
    CHECK_EQ_U(after & KNOWN, before & KNOWN);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(control87_unknown); TC_RUN(controlfp_unknown); return tc_end(); }
