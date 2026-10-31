/* TC
 * id:    ext_probe_atexit_register_during_exit
 * area:  exit_signal
 * funcs: _crt_atexit exit
 * procs: 
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.22.4.2/7.22.4.4 (registering during exit is unspecified in ISO C)
 * note:  Probes that a handler registered while exit() is already running handlers is itself run.
 */
#include "tc.h"
#include <stdlib.h>
#include <corecrt_startup.h>

static void __cdecl second(void) { _exit(31); }
static void __cdecl first(void)  { _crt_atexit(second); }

TC_CASE(late_registration_runs)
{
    DWORD c = 0; CHECK(tc_spawn_self("late", &c, 20000)); CHECK_EQ_U(c, 31);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        _crt_atexit(first);
        exit(0);
        return 77;
    }
    TC_RUN(late_registration_runs);
    return tc_end();
}
