/* TC
 * id:    nfx_signal_sigabrt_compat_probe
 * area:  negative_fatal
 * funcs: signal raise
 * procs: 
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   signal.h defines SIGABRT_COMPAT 6
 * note:  Probes: ucrtbase accepts signal number 6 (SIGABRT_COMPAT); raise(6) invokes the handler (exit code records the signal argument, 6 or 22).
 */
#include "tc.h"
#include <signal.h>

static void __cdecl h(int s) { ExitProcess((UINT)s); }
static volatile int v_sig = 6;

TC_CASE(accepted) { void (__cdecl *r)(int) = signal(v_sig, h); CHECK(r != SIG_ERR); signal(v_sig, SIG_DFL); }
TC_CASE(raise6)   { DWORD c = 0; CHECK(tc_spawn_self("r6", &c, 20000)); CHECK(c == 6 || c == 22); }

int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        signal(v_sig, h);
        raise(v_sig);
        return 77;
    }
    TC_RUN(accepted); TC_RUN(raise6);
    return tc_end();
}
