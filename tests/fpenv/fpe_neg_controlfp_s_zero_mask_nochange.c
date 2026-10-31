/* TC
 * id:    fpe_neg_controlfp_s_zero_mask_nochange
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp
 * procs: _controlfp_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _controlfp_s: new = (current & ~mask) | (newControl & mask); mask 0 changes nothing
 * note:  A nonzero newControl with mask 0 is not an error and does not touch the control word.
 */
#include "tc.h"
#include <float.h>
#include <errno.h>
TC_DECL_PROC(errno_t, _controlfp_s, (unsigned int *, unsigned int, unsigned int));

TC_CASE(zero_mask)
{
    unsigned int cur = 0xdeadbeef, before, after; errno_t r;
    volatile unsigned int nc = _RC_CHOP | _EM_INVALID | _DN_FLUSH;
    if (!tc_p__controlfp_s) TC_SKIP("not exported");
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    before = _controlfp(0, 0);
    r = tc_p__controlfp_s(&cur, nc, 0);
    after = _controlfp(0, 0);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_U(cur, before);
    CHECK_EQ_U(after, before);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(errno, 0);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_controlfp_s); TC_RUN(zero_mask); return tc_end(); }
