/* TC
 * id:    nfx_signal_handler_reset_probe
 * area:  negative_fatal
 * funcs: signal raise
 * procs: 
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.14.1.1 (implementation may reset to SIG_DFL or block)
 * note:  Probes: Microsoft CRT resets the disposition to SIG_DFL before invoking a handler (inside the handler signal(sig,SIG_DFL) returns SIG_DFL).
 */
#include "tc.h"
#include <signal.h>

static void __cdecl h(int s)
{
    void (__cdecl *prev)(int) = signal(s, SIG_DFL);
    ExitProcess(prev == SIG_DFL ? 70 : 71);
}
TC_CASE(reset) { DWORD c = 0; CHECK(tc_spawn_self("reset", &c, 20000)); CHECK_EQ_U(c, 70); }

int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        signal(SIGTERM, h);
        raise(SIGTERM);
        return 77;
    }
    TC_RUN(reset);
    return tc_end();
}
