/* TC
 * id:    ext_signal_install_hw
 * area:  exit_signal
 * funcs: signal
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs signal (SIGILL, SIGFPE, SIGSEGV supported)
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
    CHECK(sg(sig, SIG_IGN) == h);
    CHECK(sg(sig, SIG_DFL) == SIG_IGN);
}
TC_CASE(sigill) { one(SIGILL); }
TC_CASE(sigfpe) { one(SIGFPE); }
TC_CASE(sigsegv) { one(SIGSEGV); }
int main(void) { tc_begin(); TC_RUN(sigill); TC_RUN(sigfpe); TC_RUN(sigsegv); return tc_end(); }
