/* TC
 * id:    nfx_signal_sig_ign_raise
 * area:  negative_fatal
 * funcs: signal raise
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.14.2.1, 7.14.1.1 (SIG_IGN: signal is ignored)
 * note:  none
 */
#include "tc.h"
#include <signal.h>

static volatile int v_sig, v_after;
static void ign(int s)
{
    v_sig = s;
    CHECK(signal(v_sig, SIG_IGN) == SIG_DFL);
    CHECK_EQ_I(raise(v_sig), 0);
    v_after = 1;
    CHECK(signal(v_sig, SIG_DFL) == SIG_IGN);
}
TC_CASE(sigterm) { v_after = 0; ign(SIGTERM); CHECK_EQ_I(v_after, 1); }
TC_CASE(sigint)  { v_after = 0; ign(SIGINT);  CHECK_EQ_I(v_after, 1); }
TC_CASE(sigabrt) { v_after = 0; ign(SIGABRT); CHECK_EQ_I(v_after, 1); }

int main(void)
{
    tc_begin();
    TC_RUN(sigterm); TC_RUN(sigint); TC_RUN(sigabrt);
    return tc_end();
}
