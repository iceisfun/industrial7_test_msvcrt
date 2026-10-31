/* TC
 * id:    ext_neg_abort_skips_atexit_code3
 * area:  exit_signal
 * funcs: abort
 * procs: _set_abort_behavior _onexit
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs abort: terminates without running atexit/_onexit handlers; exit code 3
 * note:  ucrtbase has no _onexit export (inline in the static startup lib), so see ext_neg_abort_skips_crt_atexit_code3 for it.
 *        Child registers an _onexit handler that would ExitProcess(99). abort() must not run it; exit code stays 3.
 */
#include "tc.h"
#include <stdlib.h>

typedef int (__cdecl *onexit_fn)(void);
typedef onexit_fn (__cdecl *onexit_pf)(onexit_fn);
typedef unsigned int (__cdecl *sab_t)(unsigned int, unsigned int);
static int __cdecl handler(void) { ExitProcess(99); return 0; }

TC_CASE(handler_not_run)
{ DWORD c = 0; CHECK(tc_spawn_self("ab", &c, 20000)); CHECK_EQ_U(c, 3); }

int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        sab_t sab = (sab_t)tc_crt_proc("_set_abort_behavior");
        if (!sab) return 78;
        SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
        sab(0, 0x1 | 0x2);   /* clear _WRITE_ABORT_MSG | _CALL_REPORTFAULT */
        { onexit_pf oe = (onexit_pf)tc_crt_proc("_onexit"); if (!oe || oe(handler) == NULL) return 78; }
        abort();
        return 77;
    }
    TC_RUN(handler_not_run);
    return tc_end();
}
