/* TC
 * id:    ext_neg_quick_exit_skips_atexit
 * area:  exit_signal
 * funcs: quick_exit _crt_atexit
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.4.7 quick_exit: atexit handlers are not called; MS docs quick_exit
 * note:  Child runs in its own process.
 */
#include "tc.h"
#include <stdlib.h>
#include <corecrt_startup.h>

static void __cdecl h_atexit(void) { ExitProcess(99); }

TC_CASE(skips_atexit)
{
    DWORD code = 0; int ran = tc_spawn_self("qe", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 5);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        volatile int st = 5;
        _crt_atexit(h_atexit);
        quick_exit(st);
        return 77;
    }
    TC_RUN(skips_atexit);
    return tc_end();
}
