/* TC
 * id:    nfx_neg_signal_sigint_raise_default
 * area:  negative_fatal
 * kind:  negative
 * funcs: raise signal
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs raise, signal (SIGINT default action terminates the program; SIG_IGN ignores it); C11 7.14.2.1
 * note:  Exact exit code of the default action is not asserted (documented as exit code 3 for abort-like termination; only nonzero is asserted).
 */
#include "tc.h"
#include <signal.h>

TC_CASE(default_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("dfl", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
TC_CASE(ignored_continues)
{
    DWORD code = 99; int ran = tc_spawn_self("ign", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0);
}
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 'd') { raise(SIGINT); return 77; }
        if (signal(SIGINT, SIG_IGN) == SIG_ERR) return 1;
        if (raise(SIGINT) != 0) return 2;
        return 0;
    }
    TC_RUN(default_terminates); TC_RUN(ignored_continues);
    return tc_end();
}
