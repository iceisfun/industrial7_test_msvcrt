/* TC
 * id:    ext_neg_exit_skips_quick_exit_handlers
 * area:  exit_signal
 * funcs: exit _crt_at_quick_exit
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.4.4 exit: only atexit handlers run; at_quick_exit handlers are for quick_exit only
 */
#include "tc.h"
#include <stdlib.h>
#include <corecrt_startup.h>

static void __cdecl h_quick(void) { ExitProcess(99); }

TC_CASE(exit_skips_quick)
{
    DWORD code = 0; int ran = tc_spawn_self("ex", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 6);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        volatile int st = 6;
        _crt_at_quick_exit(h_quick);
        exit(st);
        return 77;
    }
    TC_RUN(exit_skips_quick);
    return tc_end();
}
