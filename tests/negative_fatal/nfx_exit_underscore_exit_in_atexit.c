/* TC
 * id:    nfx_exit_underscore_exit_in_atexit
 * area:  negative_fatal
 * kind:  positive
 * funcs: _exit  exit
 * procs: _onexit
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _exit
 * note:  _exit inside a handler ends the process at once; earlier-registered handlers never run.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(_onexit_t, _onexit, (_onexit_t));


static int __cdecl h1(void) { _exit(77); return 0; }  /* registered first, runs last: never reached */
static int __cdecl h2(void) { _exit(8); return 0; }

TC_CASE(underscore_exit_in_handler)
{
    DWORD code = 0xdead; int ran = tc_spawn_self("go", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 8);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_onexit);
    if (tc_child_name() != NULL) {
        if (!tc_p__onexit(h1) || !tc_p__onexit(h2)) return 50;
        exit(0);
    }
    TC_RUN(underscore_exit_in_handler);
    return tc_end();
}
