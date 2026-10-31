/* TC
 * id:    prc_neg_exit_underscore_skips_onexit
 * area:  process
 * funcs: _exit
 * procs: _onexit _crt_atexit
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _exit: terminates the process without calling atexit or _onexit functions
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

TC_CASE(handler_skipped) { DWORD code = 0; CHECK(tc_spawn_self("uexit", &code, 20000)); CHECK_EQ_U(code, 3); }

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_onexit); TC_LOAD_PROC(_crt_atexit);
    if (tc_child_name() != NULL) { reg_handler(); _exit(3); return 77; }
    TC_RUN(handler_skipped);
    return tc_end();
}
