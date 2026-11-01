/* TC
 * id:    nfx_exit_underscore_exit_skips_atexit
 * area:  negative_fatal
 * kind:  positive
 * funcs: _exit  exit
 * procs: _crt_atexit _onexit
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _exit, _cexit ("_exit ... does not process atexit or _onexit")
 * note:  D: ucrtbase.dll does not export _onexit (old code crashed 0xc0000005 calling a NULL pointer); registration goes through tc_reg_atexit() (tc.h): the real export _crt_atexit where present, else _onexit (legacy CRTs).
 *        Observed on ucrtbase: _exit(12) skips the registered handler (exit code 12),
 *        exit(12) runs it (handler _exit(99)). Stable. Documented behaviour of _exit per MS docs _exit/_cexit.
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>



static void __cdecl h(void) { _exit(99); }

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
    if ((c = tc_child_name()) != NULL) {
        if (tc_reg_atexit(h)) return 50;
        if (c[0] == 's') _exit(12);
        exit(12);
    }
    TC_RUN(_exit_skips); TC_RUN(exit_runs);
    return tc_end();
}
