/* TC
 * id:    ucr_purecall_handler_child
 * area:  ucrt_only
 * funcs:
 * procs: _set_purecall_handler _purecall
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_purecall_handler, _purecall
 */
#include "tc.h"

typedef void (__cdecl *pch_t)(void);
typedef pch_t (__cdecl *setpc_t)(pch_t);
static void __cdecl my_pc(void) { ExitProcess(41); }
static void __cdecl my_pc2(void) { ExitProcess(42); }

TC_CASE(returns_previous)
{
    setpc_t set = (setpc_t)tc_crt_proc("_set_purecall_handler");
    pch_t old;
    if (!set) TC_SKIP("not exported");
    old = set(my_pc);
    CHECK(old == NULL);
    old = set(my_pc2);
    CHECK(old == my_pc);
    old = set(NULL);
    CHECK(old == my_pc2);
}
TC_CASE(handler_invoked)
{
    DWORD code = 0;
    CHECK(tc_spawn_self("pc", &code, 20000));
    CHECK_EQ_U(code, 41);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        setpc_t set = (setpc_t)tc_crt_proc("_set_purecall_handler");
        pch_t pc = (pch_t)tc_crt_proc("_purecall");
        if (!set || !pc) ExitProcess(90);
        set(my_pc);
        pc();
        return 77;
    }
    TC_RUN(returns_previous); TC_RUN(handler_invoked);
    return tc_end();
}
