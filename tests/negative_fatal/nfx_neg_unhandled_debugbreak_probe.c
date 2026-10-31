/* TC
 * id:    nfx_neg_unhandled_debugbreak_probe
 * area:  negative_fatal
 * funcs: signal
 * procs: 
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   NTSTATUS STATUS_BREAKPOINT
 * note:  Probes: __debugbreak with no debugger attached terminates the child with 0x80000003.
 */
#include "tc.h"
#include <signal.h>

TC_CASE(int3) { DWORD c = 0; CHECK(tc_spawn_self("int3", &c, 20000)); CHECK_EQ_U(c, 0x80000003u); }

int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        signal(SIGTERM, SIG_DFL);
        __debugbreak();
        return 77;
    }
    TC_RUN(int3);
    return tc_end();
}
