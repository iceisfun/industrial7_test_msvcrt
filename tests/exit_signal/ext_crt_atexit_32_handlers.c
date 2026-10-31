/* TC
 * id:    ext_crt_atexit_32_handlers
 * area:  exit_signal
 * funcs: _crt_atexit exit
 * crt:   >=140
 * tier:  p0
 * conf:  observed
 * ref:   C11 7.22.4.2 (atexit: at least 32 functions)
 * note:  _crt_atexit is an undocumented ucrt export; ucrt table is dynamic. one handler registered 32 times; the 32nd (last) invocation checks the count
 */
#include "tc.h"
#include <stdlib.h>

int __cdecl _crt_atexit(void (__cdecl *)(void));

static int count, in_child;
static void __cdecl h(void) { if (in_child && ++count == 32) _exit(count); }

TC_CASE(register_all) { int i, bad = 0; for (i = 0; i < 32; i++) if (_crt_atexit(h) != 0) bad++; CHECK_EQ_I(bad, 0); }
TC_CASE(child_runs_all) { DWORD code = 0; int ran = tc_spawn_self("many", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 32); }

int main(void)
{
    int i;
    tc_begin();
    if (tc_child_name() != NULL) {
        in_child = 1;
        for (i = 0; i < 32; i++) if (_crt_atexit(h) != 0) return 78;
        exit(0);
        return 77;
    }
    TC_RUN(register_all); TC_RUN(child_runs_all);
    return tc_end();
}
