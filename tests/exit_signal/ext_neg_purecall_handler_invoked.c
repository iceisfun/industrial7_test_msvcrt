/* TC
 * id:    ext_neg_purecall_handler_invoked
 * area:  exit_signal
 * kind:  negative
 * funcs: _set_purecall_handler
 * procs: _purecall
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_purecall_handler
 */
#include "tc.h"
#include <stdlib.h>
static void __cdecl ph(void) { _exit(55); }
typedef void (__cdecl *pc_t)(void);
TC_CASE(handler_runs)
{
    DWORD code = 0; CHECK(tc_spawn_self("pc", &code, 20000));
    CHECK_EQ_U(code, 55);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        pc_t pc = (pc_t)tc_crt_proc("_purecall");
        if (!pc) return 78;
        _set_purecall_handler(ph);
        pc();
        return 77;
    }
    TC_RUN(handler_runs);
    return tc_end();
}
