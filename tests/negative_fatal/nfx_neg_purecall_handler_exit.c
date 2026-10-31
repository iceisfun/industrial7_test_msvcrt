/* TC
 * id:    nfx_neg_purecall_handler_exit
 * area:  negative_fatal
 * kind:  negative
 * funcs: _purecall
 * procs: _set_purecall_handler
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_purecall_handler, _purecall
 * note:  The installed handler is called by _purecall; it exits the child with 41.
 */
#include "tc.h"

typedef void (__cdecl *pc_t)(void);
extern void __cdecl _purecall(void);
static void __cdecl h(void) { _exit(41); }

TC_CASE(handler_called)
{
    DWORD c = 0; CHECK(tc_spawn_self("pc", &c, 20000)); CHECK_EQ_U(c, 41);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        pc_t (__cdecl *set)(pc_t) = (pc_t (__cdecl *)(pc_t))tc_crt_proc("_set_purecall_handler");
        if (!set) return 78;
        set(h); _purecall(); return 77;
    }
    TC_RUN(handler_called); return tc_end();
}
