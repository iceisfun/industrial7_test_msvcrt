/* TC
 * id:    prc_neg_get_errno_null
 * area:  process
 * funcs: 
 * procs: _get_errno
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _get_errno: NULL pointer invokes the invalid parameter handler, returns EINVAL
 * note:  errno itself is not asserted.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(errno_t, _get_errno, (int *));

TC_CASE(null_ptr)
{
    errno_t r;
    if (!tc_p__get_errno) { CHECK(0); return; }
    CHECK(tc_iph_install());
    tc_iph_count = 0;
    r = tc_p__get_errno(NULL);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_get_errno); TC_RUN(null_ptr); return tc_end(); }
