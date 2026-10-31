/* TC
 * id:    fpe_neg_controlfp_s_invalid_mask
 * area:  fpenv
 * funcs: _set_invalid_parameter_handler
 * procs: _controlfp_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _controlfp_s (returns EINVAL for invalid mask/new bits, calls invalid parameter handler)
 * note:  msvcrt.dll static copy differs; omitted. Bit 0x40 is outside every _MCW_* group. State-unchanged-after-failure is NOT asserted here (see fpe_neg_controlfp_s_bad_mask_state_probe).
 */
#include "tc.h"
#include <float.h>
#include <errno.h>
TC_DECL_PROC(int, _controlfp_s, (unsigned int *, unsigned int, unsigned int));

TC_CASE(invalid_mask_bit)
{
    unsigned int cur = 0;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__controlfp_s(&cur, 0x40, 0x40), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_controlfp_s); if (!tc_p__controlfp_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; } TC_RUN(invalid_mask_bit); return tc_end(); }
