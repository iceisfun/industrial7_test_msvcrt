/* TC
 * id:    nfx_neg_signal_sigill_ud2
 * area:  negative_fatal
 * funcs: signal
 * procs: 
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs signal (SIGILL)
 * note:  Probes: an illegal-instruction fault (ud2) is delivered as SIGILL to the signal() handler.
 *        Toolchain note: test exes are built with mingw-w64, whose CRT startup (not the Microsoft CRT's _XcptFilter) installs the exception-to-signal translation; the CRT under test only stores/returns the signal() handler.
 */
#include "tc.h"
#include <signal.h>

static void __cdecl h(int s) { (void)s; ExitProcess(64); }

TC_CASE(ud2) { DWORD c = 0; CHECK(tc_spawn_self("ud2", &c, 20000)); CHECK_EQ_U(c, 64); }

int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        signal(SIGILL, h);
        __builtin_trap();
        return 77;
    }
    TC_RUN(ud2);
    return tc_end();
}
