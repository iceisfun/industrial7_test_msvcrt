/* TC
 * id:    nfx_neg_abort_behavior_no_writemsg_exit3
 * area:  negative_fatal
 * funcs: _set_abort_behavior abort
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs abort ("returns exit code 3"), _set_abort_behavior
 * note:  With _WRITE_ABORT_MSG and _CALL_REPORTFAULT cleared abort() terminates the process with exit code 3.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(exit3) { DWORD code = 0; CHECK(tc_spawn_self("abort", &code, 20000)); CHECK_EQ_U(code, 3); }
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_FAILCRITICALERRORS);
        _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
        abort();
        return 77;
    }
    TC_RUN(exit3);
    return tc_end();
}
