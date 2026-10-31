/* TC
 * id:    ext_exit_c99_basic
 * area:  exit_signal
 * funcs: _crt_atexit
 * procs: _Exit
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.4.5 _Exit: no atexit handlers, no signal handlers; MS docs _Exit
 */
#include "tc.h"
#include <stdlib.h>
#include <corecrt_startup.h>

typedef void (__cdecl *exit_t)(int);
static void __cdecl h_atexit(void) { ExitProcess(99); }

TC_CASE(skips_handlers)
{
    DWORD code = 0; int ran = tc_spawn_self("e9", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 9);
}
TC_CASE(zero)
{
    DWORD code = 7; int ran = tc_spawn_self("e0", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 0);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        volatile int st = (c[1] == '9') ? 9 : 0;
        if (c[1] == '9') _crt_atexit(h_atexit);
        ((exit_t)tc_crt_proc("_Exit"))(st);
        return 77;
    }
    TC_RUN(skips_handlers); TC_RUN(zero);
    return tc_end();
}
