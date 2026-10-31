/* TC
 * id:    fpe_neg_controlfp_s_errno_einval
 * area:  fpenv
 * kind:  negative
 * funcs:
 * procs: _controlfp_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _controlfp_s (returns EINVAL for invalid mask); Parameter validation (errno set, handler invoked)
 * note:  Bit 0x00004000 (chosen over 0x00200000: same status, already widely used) is not part of any _MCW_* field. Handler installed and returns, so the call completes.
 */
#include "tc.h"
#include <float.h>
#include <errno.h>
TC_DECL_PROC(errno_t, _controlfp_s, (unsigned int *, unsigned int, unsigned int));

TC_CASE(invalid_mask_sets_errno)
{
    unsigned int cur = 0; errno_t r; volatile unsigned int m = 0x00004000;
    if (!tc_p__controlfp_s) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0; errno = 0;
    r = tc_p__controlfp_s(&cur, m, m);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(valid_call_leaves_errno)
{
    unsigned int cur = 0; errno_t r;
    if (!tc_p__controlfp_s) TC_SKIP("not exported");
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = tc_p__controlfp_s(&cur, 0, 0);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(errno, 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_controlfp_s); TC_RUN(invalid_mask_sets_errno); TC_RUN(valid_call_leaves_errno); return tc_end(); }
