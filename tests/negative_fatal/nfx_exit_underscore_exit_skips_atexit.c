/* TC
 * id:    nfx_exit_underscore_exit_skips_atexit
 * area:  negative_fatal
 * kind:  positive
 * funcs: _exit  exit
 * procs: _onexit
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _exit, _cexit ("_exit ... does not process atexit or _onexit")
 * note:  _exit terminates without running registered handlers; exit runs them.
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

TC_DECL_PROC(_onexit_t, _onexit, (_onexit_t));


static int __cdecl h(void) { _exit(99); return 0; }

TC_CASE(_exit_skips)
{
    DWORD code = 0; int ran = tc_spawn_self("skip", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 12);
}
TC_CASE(exit_runs)
{
    DWORD code = 0; int ran = tc_spawn_self("run", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 99);
}
int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(_onexit);
    if ((c = tc_child_name()) != NULL) {
        if (!tc_p__onexit(h)) return 50;
        if (c[0] == 's') _exit(12);
        exit(12);
    }
    TC_RUN(_exit_skips); TC_RUN(exit_runs);
    return tc_end();
}
