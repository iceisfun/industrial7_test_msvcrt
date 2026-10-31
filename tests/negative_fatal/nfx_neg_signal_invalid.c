/* TC
 * id:    nfx_neg_signal_invalid
 * area:  negative_fatal
 * funcs: signal
 * procs: 
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs signal (invalid signal number: SIG_ERR, errno EINVAL, invalid parameter handler)
 * note:  none
 */
#include "tc.h"
#include <signal.h>
#include <errno.h>

static volatile int v_sig;
static void __cdecl h(int s) { (void)s; }
static void chk(int s)
{
    void (__cdecl *r)(int);
    tc_iph_count = 0; errno = 0; v_sig = s;
    r = signal(v_sig, h);
    CHECK(r == SIG_ERR);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(zero)     { chk(0); }
TC_CASE(minus1)   { chk(-1); }
TC_CASE(nine)     { chk(9); }
TC_CASE(thousand) { chk(1000); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph api\n"); return 1; }
    TC_RUN(zero); TC_RUN(minus1); TC_RUN(nine); TC_RUN(thousand);
    return tc_end();
}
