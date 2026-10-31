/* TC
 * id:    ext_probe_signal_reset_after_raise
 * area:  exit_signal
 * funcs: signal raise
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.14.1.1 permits reset to SIG_DFL; MS docs signal
 * note:  Probes that the CRT resets the handler to SIG_DFL before invoking it for a raised SIGTERM.
 */
#include "tc.h"
#include <signal.h>
static void __cdecl h(int s) { (void)s; }
typedef void (__cdecl *sh_t)(int);
static sh_t sg(int n, sh_t f) { return (sh_t)signal(n, f); }

static volatile int calls;
static void __cdecl rh(int s) { (void)s; calls++; }
TC_CASE(reset)
{
    CHECK(sg(SIGTERM, rh) == SIG_DFL);
    CHECK_EQ_I(raise(SIGTERM), 0);
    CHECK_EQ_I(calls, 1);
    CHECK(sg(SIGTERM, SIG_DFL) == SIG_DFL);
}
int main(void) { tc_begin(); TC_RUN(reset); return tc_end(); }
