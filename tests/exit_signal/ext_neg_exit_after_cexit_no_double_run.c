/* TC
 * id:    ext_neg_exit_after_cexit_no_double_run
 * area:  exit_signal
 * funcs: _cexit exit
 * procs: _onexit _crt_atexit
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _cexit, exit
 * note:  ucrtbase has no _onexit export, so _crt_atexit is used there. PROBES that handlers already run by _cexit() are not run again by a later exit(). Handler counts; on a second
 *        run it ExitProcess(102). Expected exit code: 1 (the handler ran once, during _cexit).
 */
#include "tc.h"
#include <stdlib.h>

typedef int (__cdecl *onexit_fn)(void);
typedef onexit_fn (__cdecl *onexit_pf)(onexit_fn);
static volatile int count;
static int __cdecl handler(void)
{
    count++;
    if (count >= 2) ExitProcess(102);
    return 0;
}
TC_CASE(once_only)
{ DWORD c = 0; CHECK(tc_spawn_self("cx", &c, 20000)); CHECK_EQ_U(c, 1); }
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
#if TC_CRT_VER >= 140
        {
        int (__cdecl *oe)(onexit_fn) = (int (__cdecl *)(onexit_fn))tc_crt_proc("_crt_atexit"); if (!oe || oe(handler) != 0) return 78;
        }
#else
        {
        onexit_pf oe = (onexit_pf)tc_crt_proc("_onexit"); if (!oe || oe(handler) == NULL) return 78;
        }
#endif
        _cexit();
        exit(count);
        return 77;
    }
    TC_RUN(once_only);
    return tc_end();
}
