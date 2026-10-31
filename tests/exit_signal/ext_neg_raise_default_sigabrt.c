/* TC
 * id:    ext_neg_raise_default_sigabrt
 * area:  exit_signal
 * kind:  negative
 * funcs: raise _set_abort_behavior
 * crt:   >=90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs raise / abort: abnormal termination, exit code 3
 * note:  PROBE: raise(SIGABRT) (not abort()) with SIG_DFL exits with code 3; docs state this for abort/signal defaults, not literally for raise.
 *        tc_begin already clears the abort message and reportfault flags in the child.
 */
#include "tc.h"
#include <signal.h>
#include <stdlib.h>
TC_CASE(child_exit3)
{
    DWORD code = 0; int ran = tc_spawn_self("abrt", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 3);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT); raise(SIGABRT); return 77; }
    TC_RUN(child_exit3);
    return tc_end();
}
