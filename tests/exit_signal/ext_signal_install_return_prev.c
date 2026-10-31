/* TC
 * id:    ext_signal_install_return_prev
 * area:  exit_signal
 * funcs: signal
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   C11 7.14.1.1; MS docs signal (returns previous value)
 */
#include "tc.h"
#include <signal.h>
static void __cdecl h(int s) { (void)s; }
typedef void (__cdecl *sh_t)(int);
static sh_t sg(int n, sh_t f) { return (sh_t)signal(n, f); }

static void one(int sig)
{
    CHECK(sg(sig, h) == SIG_DFL);
    CHECK(sg(sig, h) == h);
    CHECK(sg(sig, SIG_DFL) == h);
}
TC_CASE(sigint) { one(SIGINT); }
TC_CASE(sigterm) { one(SIGTERM); }
TC_CASE(sigbreak) { one(SIGBREAK); }
TC_CASE(sigabrt) { one(SIGABRT); }
int main(void) { tc_begin(); TC_RUN(sigint); TC_RUN(sigterm); TC_RUN(sigbreak); TC_RUN(sigabrt); return tc_end(); }
