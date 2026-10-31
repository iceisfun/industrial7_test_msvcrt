/* TC
 * id:    nfx_quick_exit_and_exit_noatexit
 * area:  negative_fatal
 * kind:  positive
 * funcs: quick_exit
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.4.5-7.22.4.7; MS docs quick_exit, _Exit, _crt_at_quick_exit
 * note:  Children terminate via _Exit/quick_exit; handlers that would change the exit code prove which handlers ran.
 */
#include "tc.h"
#include <stdlib.h>


typedef void (__cdecl *vfn_t)(void);
TC_DECL_PROC(void, _Exit, (int));
TC_DECL_PROC(int, _crt_at_quick_exit, (vfn_t));
TC_DECL_PROC(int, _crt_atexit, (vfn_t));

static void __cdecl qh12(void) { _exit(12); }
static void __cdecl qh_noop(void) { }
static void __cdecl exit_handler99(void) { _exit(99); }

TC_CASE(registers_ok) { CHECK_EQ_I(tc_p__crt_at_quick_exit(qh_noop), 0); }
TC_CASE(_Exit_skips_handlers)
{
    DWORD c = 0; CHECK(tc_spawn_self("_exit9", &c, 20000)); CHECK_EQ_U(c, 9);
}
TC_CASE(quick_exit_runs_quick_handler)
{
    DWORD c = 0; CHECK(tc_spawn_self("qe_handler", &c, 20000)); CHECK_EQ_U(c, 12);
}
TC_CASE(quick_exit_skips_onexit)
{
    DWORD c = 0; CHECK(tc_spawn_self("qe_onexit", &c, 20000)); CHECK_EQ_U(c, 10);
}
int main(void)
{
    const char *n;
    tc_begin();
    TC_LOAD_PROC(_Exit); TC_LOAD_PROC(_crt_at_quick_exit); TC_LOAD_PROC(_crt_atexit);
    if (!tc_p__Exit || !tc_p__crt_at_quick_exit || !tc_p__crt_atexit) { tc_write("TC FAIL missing export\n"); return 2; }
    if ((n = tc_child_name()) != NULL) {
        volatile int s = 0;
        if (n[0] == '_') { tc_p__Exit(9 + s); }
        if (n[3] == 'h') { tc_p__crt_at_quick_exit(qh12); quick_exit(10 + s); }
        tc_p__crt_atexit(exit_handler99);
        quick_exit(10 + s);
        return 77;
    }
    TC_RUN(registers_ok); TC_RUN(_Exit_skips_handlers); TC_RUN(quick_exit_runs_quick_handler); TC_RUN(quick_exit_skips_onexit);
    return tc_end();
}
