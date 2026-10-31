/* TC
 * id:    ext_raise_calls_handler
 * area:  exit_signal
 * funcs: signal raise
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.14.2.1 raise; MS docs raise
 */
#include "tc.h"
#include <signal.h>
static void __cdecl h(int s) { (void)s; }
typedef void (__cdecl *sh_t)(int);
static sh_t sg(int n, sh_t f) { return (sh_t)signal(n, f); }

static volatile int seen, calls;
static void __cdecl rh(int s) { seen = s; calls++; }
static void one(int sig)
{
    seen = 0; calls = 0;
    CHECK(sg(sig, rh) == SIG_DFL);
    CHECK_EQ_I(raise(sig), 0);
    CHECK_EQ_I(calls, 1);
    CHECK_EQ_I(seen, sig);
    sg(sig, SIG_DFL);
}
TC_CASE(sigint) { one(SIGINT); }
TC_CASE(sigterm) { one(SIGTERM); }
TC_CASE(sigbreak) { one(SIGBREAK); }
TC_CASE(sigabrt) { one(SIGABRT); }
TC_CASE(sigill) { one(SIGILL); }
TC_CASE(sigsegv) { one(SIGSEGV); }
int main(void) { tc_begin(); TC_RUN(sigint); TC_RUN(sigterm); TC_RUN(sigbreak); TC_RUN(sigabrt); TC_RUN(sigill); TC_RUN(sigsegv); return tc_end(); }
