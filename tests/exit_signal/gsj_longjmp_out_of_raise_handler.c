/* TC
 * id:    gsj_longjmp_out_of_raise_handler
 * area:  exit_signal
 * funcs: longjmp raise signal
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.13.2.1, 7.14.2.1; MS docs signal ("before the handler is called the signal is reset to SIG_DFL"), raise
 * note:  Never uses SIGABRT. Synchronous raise() only. Case reset_to_default is the documented-by-MS part.
 */
#include "tc.h"
#include <setjmp.h>
#include <signal.h>

static jmp_buf jb;
static volatile int in_handler;

static void h(int sig) { in_handler = sig; longjmp(jb, 2); }

TC_CASE(sigterm)
{
    volatile int sig = SIGTERM; in_handler = 0;
    int r = setjmp(jb);
    if (r == 0) { signal(sig, h); raise(sig); }
    CHECK_EQ_I(r, 2); CHECK_EQ_I(in_handler, SIGTERM);
    signal(SIGTERM, SIG_DFL);
}
TC_CASE(sigint)
{
    volatile int sig = SIGINT; in_handler = 0;
    int r = setjmp(jb);
    if (r == 0) { signal(sig, h); raise(sig); }
    CHECK_EQ_I(r, 2); CHECK_EQ_I(in_handler, SIGINT);
    signal(SIGINT, SIG_DFL);
}
TC_CASE(reset_to_default)
{
    volatile int sig = SIGTERM;
    void (*prev)(int);
    int r = setjmp(jb);
    if (r == 0) { signal(sig, h); raise(sig); }
    prev = signal(SIGTERM, SIG_DFL);
    CHECK(prev == SIG_DFL);
}

int main(void)
{
    tc_begin();
    TC_RUN(sigterm); TC_RUN(sigint); TC_RUN(reset_to_default);
    return tc_end();
}
