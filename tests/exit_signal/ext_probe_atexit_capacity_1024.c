/* TC
 * id:    ext_probe_atexit_capacity_1024
 * area:  exit_signal
 * funcs: _crt_atexit exit
 * procs: 
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.22.4.2: implementation shall support at least 32 atexit registrations
 * note:  Probes that the UCRT table grows beyond 32 (to 1024) and every handler runs on exit.
 */
#include "tc.h"
#include <stdlib.h>
#include <corecrt_startup.h>

#define N 1024
static volatile int count;
static void __cdecl h(void) { count++; }
static void __cdecl last(void) { _exit(count == N ? 0 : 50); } /* registered first => runs last */

TC_CASE(register_ok) { DWORD c = 1; CHECK(tc_spawn_self("reg", &c, 20000)); CHECK_EQ_U(c, 0); }
TC_CASE(all_run)     { DWORD c = 1; CHECK(tc_spawn_self("run", &c, 20000)); CHECK_EQ_U(c, 0); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        int i, bad = 0;
        if (c[0] == 'r' && c[1] == 'u') _crt_atexit(last);
        for (i = 0; i < N; i++) if (_crt_atexit(h) != 0) bad++;
        if (c[1] == 'e') _exit(bad ? 40 : 0);
        if (bad) _exit(40);
        exit(60);
    }
    TC_RUN(register_ok); TC_RUN(all_run);
    return tc_end();
}
