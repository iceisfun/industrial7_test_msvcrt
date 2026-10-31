/* TC
 * id:    cpp_neg_purecall_handler_invoked_child
 * area:  cpp_runtime
 * funcs: _purecall
 * procs: _set_purecall_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_purecall_handler
 * note:  The handler replaces the default report-and-abort action.
 */
#include "tc.h"

typedef void (__cdecl *pc_t)(void);
void __cdecl _purecall(void);
static void __cdecl h(void) { ExitProcess(42); }

TC_CASE(handler_runs)
{
    DWORD code = 0; int ran = tc_spawn_self("pure", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 42);
}
TC_CASE(handler_before_abort)
{
    DWORD code = 0; int ran = tc_spawn_self("pure", &code, 20000);
    CHECK(ran);
    CHECK(code != 3);
    CHECK(code != 77);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        pc_t (__cdecl *set)(pc_t) = (pc_t (__cdecl *)(pc_t))tc_crt_proc("_set_purecall_handler");
        if (!set) return 90;
        set(h);
        _purecall();
        return 77;
    }
    TC_RUN(handler_runs); TC_RUN(handler_before_abort);
    return tc_end();
}
