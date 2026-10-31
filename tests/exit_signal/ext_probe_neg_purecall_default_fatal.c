/* TC
 * id:    ext_probe_neg_purecall_default_fatal
 * area:  exit_signal
 * kind:  negative
 * funcs: 
 * procs: _purecall
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _purecall (R6025)
 * note:  PROBE: default _purecall (no handler) terminates the process with fast-fail status 0xC0000409 when abort reportfault is enabled. Child re-enables _CALL_REPORTFAULT; error boxes suppressed. Termination itself (not 0/77/78) is asserted in a separate case.
 */
#include "tc.h"
#include <stdlib.h>
typedef void (__cdecl *pc_t)(void);
TC_CASE(terminates)
{
    DWORD code = 0; CHECK(tc_spawn_self("pc", &code, 20000));
    CHECK(code != 0); CHECK(code != 77); CHECK(code != 78);
}
TC_CASE(fastfail_status)
{
    DWORD code = 0; CHECK(tc_spawn_self("pc", &code, 20000));
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        pc_t pc = (pc_t)tc_crt_proc("_purecall");
        if (!pc) return 78;
        SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
        _set_abort_behavior(_CALL_REPORTFAULT, _CALL_REPORTFAULT);
        pc();
        return 77;
    }
    TC_RUN(terminates); TC_RUN(fastfail_status);
    return tc_end();
}
