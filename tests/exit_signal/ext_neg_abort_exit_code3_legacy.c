/* TC
 * id:    ext_neg_abort_exit_code3_legacy
 * area:  exit_signal
 * funcs: abort
 * procs: 
 * crt:   msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs abort: returns exit code 3; SetErrorMode(SEM_NOGPFAULTERRORBOX) suppresses the Watson box
 * note:  Child process. msvcrt.dll only (no _set_abort_behavior, no Watson path). msvcr80+ abort() defaults to _CALL_REPORTFAULT
 *        which terminates with a fault status rather than 3 unless disabled; that is covered by ext_neg_abort_exit_code3. The abort message may be written to stderr; ignored.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(code3)
{ DWORD c = 0; CHECK(tc_spawn_self("ab", &c, 20000)); CHECK_EQ_U(c, 3); }

int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
        abort();
        return 77;
    }
    TC_RUN(code3);
    return tc_end();
}
