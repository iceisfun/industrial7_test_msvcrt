/* TC
 * id:    nfx_neg_raise_invalid_signal
 * area:  negative_fatal
 * funcs: raise
 * procs: 
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs raise (nonzero return on failure); MS docs signal (EINVAL, invalid parameter handler)
 * note:  MS docs state EINVAL/invalid-parameter-handler explicitly for signal(); for raise() they only say 'nonzero on failure'. The EINVAL + handler assertions mirror signal() behaviour (inferred). Signals 0,-1,3,9,1000 are not in the valid set (2,4,6(ucrt),8,11,15,21,22).
 */
#include "tc.h"
#include <signal.h>
#include <errno.h>

static volatile int v_sig;
static void chk(int s)
{
    int r;
    tc_iph_count = 0; errno = 0; v_sig = s;
    r = raise(v_sig);
    CHECK(r != 0);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(zero)     { chk(0); }
TC_CASE(minus1)   { chk(-1); }
TC_CASE(three)    { chk(3); }
TC_CASE(nine)     { chk(9); }
TC_CASE(thousand) { chk(1000); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph api\n"); return 1; }
    TC_RUN(zero); TC_RUN(minus1); TC_RUN(three); TC_RUN(nine); TC_RUN(thousand);
    return tc_end();
}
