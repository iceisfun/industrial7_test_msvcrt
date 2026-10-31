/* TC
 * id:    cpp_neg_purecall_handler_returns_probe_child
 * area:  cpp_runtime
 * funcs: _purecall
 * procs: _set_purecall_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _set_purecall_handler (handler is not expected to return)
 * note:  PROBE: a purecall handler that returns. Asserted: the CRT falls back to its default action after the handler returns
 *        (report + abort(), exit 3 with _CALL_REPORTFAULT cleared). Alternative real outcome: control returns from _purecall (exit 50).
 */
#include "tc.h"

typedef void (__cdecl *pc_t)(void);
void __cdecl _purecall(void);
static volatile int flag;
static void __cdecl h(void) { flag = 1; }

TC_CASE(handler_returns)
{
    DWORD code = 0; int ran = tc_spawn_self("pure", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 3);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        pc_t (__cdecl *set)(pc_t) = (pc_t (__cdecl *)(pc_t))tc_crt_proc("_set_purecall_handler");
        if (!set) return 90;
        set(h);
        _purecall();
        return flag ? 50 : 51;
    }
    TC_RUN(handler_returns);
    return tc_end();
}
