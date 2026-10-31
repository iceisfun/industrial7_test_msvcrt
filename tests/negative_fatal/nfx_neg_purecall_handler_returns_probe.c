/* TC
 * id:    nfx_neg_purecall_handler_returns_probe
 * area:  negative_fatal
 * kind:  negative
 * funcs: _purecall
 * procs: _set_purecall_handler
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_purecall_handler
 * note:  PROBES what happens when the purecall handler returns: assumed the process still terminates (not 77, not 0). Exact status is not asserted; only != 0 and != 77 (i.e. _purecall did not return normally to the caller). Believed: msvcr80..120 fall through to abort(); ucrtbase unsure whether it returns or fails fast.
 */
#include "tc.h"

typedef void (__cdecl *pc_t)(void);
extern void __cdecl _purecall(void);
static void __cdecl h(void) { }

TC_CASE(terminates_anyway)
{
    DWORD c = 0; CHECK(tc_spawn_self("pc", &c, 20000)); CHECK(c != 77); CHECK(c != 0);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        pc_t (__cdecl *set)(pc_t) = (pc_t (__cdecl *)(pc_t))tc_crt_proc("_set_purecall_handler");
        if (!set) return 78;
        set(h); _purecall(); return 77;
    }
    TC_RUN(terminates_anyway); return tc_end();
}
