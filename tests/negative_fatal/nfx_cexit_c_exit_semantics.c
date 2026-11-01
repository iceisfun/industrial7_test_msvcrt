/* TC
 * id:    nfx_cexit_c_exit_semantics
 * area:  negative_fatal
 * kind:  positive
 * funcs: _cexit _c_exit  _exit
 * procs: _onexit _crt_atexit
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
#if TC_CRT_VER >= 140
/* ucrtbase does NOT export _onexit (class D: the old GetProcAddress("_onexit") gave NULL and every call crashed with 0xC0000005);
 * the toolchain's static _onexit registers through the exported _crt_atexit. Older CRTs still export it. */
#define TC_ONEXIT(f) _onexit(f)
#else
#define TC_ONEXIT(f) tc_p__onexit(f)
#endif


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
    TC_LOAD_PROC(_onexit);   /* NULL on ucrtbase, unused there */
    if ((c = tc_child_name()) != NULL) {
        if (!TC_ONEXIT(h)) return 50;
        if (c[1] == 'e') { _cexit(); _exit(flag ? 20 : 21); }
        _c_exit();
        _exit(flag ? 23 : 22);
    }
    TC_RUN(cexit_runs_handlers); TC_RUN(c_exit_no_handlers);
    return tc_end();
}
