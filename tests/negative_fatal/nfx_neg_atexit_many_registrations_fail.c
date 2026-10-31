/* TC
 * id:    nfx_neg_atexit_many_registrations_fail
 * area:  negative_fatal
 * kind:  negative
 * funcs: _crt_atexit
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.22.4.2 (at least 32 registrations supported; nonzero return when registration fails); MS docs atexit
 * note:  Probes: registering up to 100000 handlers never crashes; _crt_atexit (the function behind atexit) either keeps returning 0 (table grows) or returns nonzero
 *        (out of memory) - the loop stops at the first nonzero. At least 32 must have succeeded (C11 minimum). All registered
 *        handlers (no-ops) then run at normal exit with code 0.
 */
#include "tc.h"
#include <stdlib.h>

int __cdecl _crt_atexit(void (__cdecl *)(void));

static void __cdecl nop(void) { }

TC_CASE(register_100000)
{
    DWORD code = 99; int ran = tc_spawn_self("many", &code, 30000);
    CHECK(ran); CHECK_EQ_U(code, 0);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        volatile int count = 0, i;
        for (i = 0; i < 100000; i++) { if (_crt_atexit(nop) != 0) break; count++; }
        return count >= 32 ? 0 : 70;
    }
    TC_RUN(register_100000);
    return tc_end();
}
