/* TC
 * id:    nfx_cexit_c_exit_semantics
 * area:  negative_fatal
 * kind:  positive
 * funcs: _cexit _c_exit  _exit
 * procs: _onexit
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _cexit, _c_exit
 * note:  _cexit runs onexit handlers and returns without terminating; _c_exit performs cleanup but runs no handlers and does not terminate.
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

TC_DECL_PROC(_onexit_t, _onexit, (_onexit_t));


static volatile int flag;
static int __cdecl h(void) { flag = 1; return 0; }

TC_CASE(cexit_runs_handlers)
{
    DWORD code = 0; int ran = tc_spawn_self("cexit", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 20);
}
TC_CASE(c_exit_no_handlers)
{
    DWORD code = 0; int ran = tc_spawn_self("c_exit", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 22);
}
int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(_onexit);
    if ((c = tc_child_name()) != NULL) {
        if (!tc_p__onexit(h)) return 50;
        if (c[1] == 'e') { _cexit(); _exit(flag ? 20 : 21); }
        _c_exit();
        _exit(flag ? 23 : 22);
    }
    TC_RUN(cexit_runs_handlers); TC_RUN(c_exit_no_handlers);
    return tc_end();
}
