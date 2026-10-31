/* TC
 * id:    prc_exit_runs_onexit
 * area:  process
 * funcs: exit
 * procs: _onexit _crt_atexit
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs exit, _onexit: exit calls registered functions before terminating
 * note:  Child registers a handler that ends the process with code 9, then terminates normally.
 */
#include "tc.h"
#include <stdlib.h>

TC_DECL_PROC(_onexit_t, _onexit, (_onexit_t));
TC_DECL_PROC(int, _crt_atexit, (void (__cdecl *)(void)));
static int __cdecl handler(void) { ExitProcess(9); return 0; }
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

TC_CASE(exit_call)   { DWORD code = 0; CHECK(tc_spawn_self("viaexit", &code, 20000)); CHECK_EQ_U(code, 9); }
TC_CASE(main_return) { DWORD code = 0; CHECK(tc_spawn_self("viaret", &code, 20000));  CHECK_EQ_U(code, 9); }

int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(_onexit); TC_LOAD_PROC(_crt_atexit);
    if ((c = tc_child_name()) != NULL) {
        reg_handler();
        if (c[3] == 'e') exit(0);   /* "viaexit" */
        return 0;                   /* "viaret" */
    }
    TC_RUN(exit_call); TC_RUN(main_return);
    return tc_end();
}
