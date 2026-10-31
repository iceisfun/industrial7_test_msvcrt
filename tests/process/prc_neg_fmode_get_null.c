/* TC
 * id:    prc_neg_fmode_get_null
 * area:  process
 * funcs: 
 * procs: _get_fmode
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _get_fmode: NULL pValue invokes invalid parameter handler, returns EINVAL
 */
#include "tc.h"
#include <errno.h>
TC_DECL_PROC(errno_t, _get_fmode, (int *));
TC_CASE(null_arg)
{
    errno_t r;
    if (!tc_p__get_fmode) TC_SKIP("_get_fmode not exported");
    if (!tc_iph_install()) TC_SKIP("no iph api");
    tc_iph_count = 0;
    r = tc_p__get_fmode(NULL);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_get_fmode); TC_RUN(null_arg); return tc_end(); }
