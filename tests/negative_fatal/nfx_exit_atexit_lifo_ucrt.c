/* TC
 * id:    nfx_exit_atexit_lifo_ucrt
 * area:  negative_fatal
 * kind:  positive
 * funcs: _crt_atexit
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   C11 7.22.4.2; UCRT corecrt_startup.h
 * note:  _crt_atexit is the export used by the atexit inline in <corecrt_startup.h>.
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

#define CALL(f) _crt_atexit(f)

static volatile int state;
static void __cdecl h1(void) { state = state * 10 + 1; _exit(state); }
static void __cdecl h2(void) { state = state * 10 + 2; }
static void __cdecl h3(void) { state = state * 10 + 3; }

TC_CASE(atexit_returns_zero)
{
    DWORD code = 0xdead; int ran = tc_spawn_self("ret", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0);
}
TC_CASE(atexit_lifo)
{
    DWORD code = 0; int ran = tc_spawn_self("lifo", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 321);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        int r1 = CALL(h1), r2 = CALL(h2), r3 = CALL(h3);
        if (c[0] == 'r') _exit((r1 | r2 | r3) ? 1 : 0);
        exit(0);
    }
    TC_RUN(atexit_returns_zero); TC_RUN(atexit_lifo);
    return tc_end();
}
