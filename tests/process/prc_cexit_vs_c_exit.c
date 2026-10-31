/* TC
 * id:    prc_cexit_vs_c_exit
 * area:  process
 * funcs: _cexit _c_exit
 * procs: _onexit _crt_atexit
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _cexit, _c_exit: _cexit runs onexit handlers and returns; _c_exit does the same cleanup without running them
 */
#include "tc.h"
#include <stdlib.h>
#include <process.h>

TC_DECL_PROC(_onexit_t, _onexit, (_onexit_t));
TC_DECL_PROC(int, _crt_atexit, (void (__cdecl *)(void)));
static int __cdecl handler(void) { ExitProcess(21); return 0; }
static void __cdecl vhandler(void) { handler(); }
/* ucrtbase: register through _crt_atexit; older CRT DLLs: _onexit. */
static void reg_handler(void)
{
#if TC_CRT_VER >= 140
    tc_p__crt_atexit(vhandler);
#else
    tc_p__onexit((_onexit_t)handler);
#endif
}

TC_CASE(cexit_runs_handlers)  { DWORD code = 0; CHECK(tc_spawn_self("cexit", &code, 20000));  CHECK_EQ_U(code, 21); }
TC_CASE(c_exit_skips_handlers){ DWORD code = 0; CHECK(tc_spawn_self("c_exit", &code, 20000)); CHECK_EQ_U(code, 22); }

int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(_onexit); TC_LOAD_PROC(_crt_atexit);
    if ((c = tc_child_name()) != NULL) {
        reg_handler();
        if (c[1] == 'e') _cexit(); else _c_exit();   /* "cexit" vs "c_exit" */
        ExitProcess(22);
    }
    TC_RUN(cexit_runs_handlers); TC_RUN(c_exit_skips_handlers);
    return tc_end();
}
