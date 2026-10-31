/* TC
 * id:    ext_probe_exitprocess_skips_atexit
 * area:  exit_signal
 * funcs: 
 * procs: _onexit _crt_atexit
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   Win32 ExitProcess vs CRT atexit (not documented by the CRT)
 * note:  Probes that calling ExitProcess directly does not run handlers registered with the CRT's _onexit
 *        (handler would _exit(99)); expected exit code 12.
 */
#include "tc.h"
#include <stdlib.h>

typedef int (__cdecl *onexit_fn)(void);
typedef onexit_fn (__cdecl *onexit_pf)(onexit_fn);
typedef int (__cdecl *atexit_pf)(void (__cdecl *)(void));
static int __cdecl h(void) { _exit(99); return 0; }

TC_CASE(skips)
{
    DWORD c = 0; CHECK(tc_spawn_self("ep", &c, 20000)); CHECK_EQ_U(c, 12);
}
int main(void)
{
    onexit_pf f;
    tc_begin();
    f = (onexit_pf)tc_crt_proc("_onexit");
    if (tc_child_name()) {
        atexit_pf ae = (atexit_pf)tc_crt_proc("_crt_atexit");
        if (f) f(h);
        else if (ae) ae((void (__cdecl *)(void))h);
        else return 13;
        ExitProcess(12);
    }
    TC_RUN(skips);
    return tc_end();
}
