/* TC
 * id:    ext_neg_signal_ign_then_invalid_state
 * area:  exit_signal
 * funcs: signal raise
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs signal, raise: SIG_IGN ignores the signal; signal returns the previous handler
 * note:  After SIG_IGN the earlier handler must not run even though raise returns 0.
 */
#include "tc.h"
#include <signal.h>

static volatile int cnt;
static void __cdecl h(int s) { (void)s; cnt++; }

static void run(int sig)
{
    volatile int sn = sig;
    cnt = 0;
    CHECK(signal(sn, h) == SIG_DFL);
    CHECK(signal(sn, SIG_IGN) == h);
    CHECK_EQ_I(raise(sn), 0);
    CHECK_EQ_I(cnt, 0);
    CHECK(signal(sn, SIG_DFL) == SIG_IGN);
    CHECK(signal(sn, SIG_DFL) == SIG_DFL);
}
TC_CASE(sigint)   { run(SIGINT); }
TC_CASE(sigterm)  { run(SIGTERM); }
TC_CASE(sigbreak) { run(SIGBREAK); }
TC_CASE(sigabrt)  { run(SIGABRT); }
int main(void) { tc_begin(); TC_RUN(sigint); TC_RUN(sigterm); TC_RUN(sigbreak); TC_RUN(sigabrt); return tc_end(); }
