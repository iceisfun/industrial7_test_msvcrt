/* TC
 * id:    nfx_signal_return_previous
 * area:  negative_fatal
 * funcs: signal
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.14.1.1 (signal returns the previous handler); MS docs signal
 * note:  in-process; every signal is restored to SIG_DFL at the end
 */
#include "tc.h"
#include <signal.h>

typedef void (__cdecl *sh_t)(int);
static void __cdecl h1(int s) { (void)s; }
static void __cdecl h2(int s) { (void)s; }
static volatile int v_sig;

static void chain(int s)
{
    v_sig = s;
    CHECK(signal(v_sig, h1) == SIG_DFL);
    CHECK(signal(v_sig, h2) == h1);
    CHECK(signal(v_sig, SIG_IGN) == h2);
    CHECK(signal(v_sig, SIG_DFL) == SIG_IGN);
    CHECK(signal(v_sig, SIG_DFL) == SIG_DFL);
}
TC_CASE(sigterm) { chain(SIGTERM); }
TC_CASE(sigint)  { chain(SIGINT); }
TC_CASE(sigabrt) { chain(SIGABRT); }

int main(void)
{
    tc_begin();
    TC_RUN(sigterm); TC_RUN(sigint); TC_RUN(sigabrt);
    signal(SIGTERM, SIG_DFL); signal(SIGINT, SIG_DFL); signal(SIGABRT, SIG_DFL);
    return tc_end();
}
