/* TC
 * id:    ext_neg_raise_default_sigterm
 * area:  exit_signal
 * kind:  negative
 * funcs: raise
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   MS docs raise: default action terminates with exit code 3
 * note:  PROBE: unhandled raise(SIGTERM) with SIG_DFL terminates the process with exit code 3.
 */
#include "tc.h"
#include <signal.h>
TC_CASE(child_exit3)
{
    DWORD code = 0; int ran = tc_spawn_self("term", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 3);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { volatile int s = SIGTERM; raise(s); return 77; }
    TC_RUN(child_exit3);
    return tc_end();
}
