/* TC
 * id:    ext_raise_sigfpe_handler
 * area:  exit_signal
 * funcs: signal raise
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.14.2.1; MS docs raise/signal (SIGFPE)
 */
#include "tc.h"
#include <signal.h>
static void __cdecl h(int s) { (void)s; }
typedef void (__cdecl *sh_t)(int);
static sh_t sg(int n, sh_t f) { return (sh_t)signal(n, f); }

static volatile int seen, calls;
static void __cdecl rh(int s) { seen = s; calls++; }
TC_CASE(fpe)
{
    CHECK(sg(SIGFPE, rh) == SIG_DFL);
    CHECK_EQ_I(raise(SIGFPE), 0);
    CHECK_EQ_I(calls, 1);
    CHECK_EQ_I(seen, SIGFPE);
    sg(SIGFPE, SIG_DFL);
}
int main(void) { tc_begin(); TC_RUN(fpe); return tc_end(); }
