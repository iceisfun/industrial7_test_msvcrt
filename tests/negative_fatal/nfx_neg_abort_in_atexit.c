/* TC
 * id:    nfx_neg_abort_in_atexit
 * area:  negative_fatal
 * kind:  negative
 * funcs: abort
 * procs: _onexit _crt_atexit

 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _onexit, abort
 * note:  PROBE: abort() from an exit handler run by exit(0) terminates with code 3.
 */
#include "tc.h"
#include <stdlib.h>
#include <signal.h>

static int __cdecl hx(void) { abort(); return 0; }
static void __cdecl hv(void) { abort(); }
TC_CASE(abort_in_handler)
{
    DWORD code = 0; int ran = tc_spawn_self("ax", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
    CHECK_EQ_U(code, 3);
}
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL) {
#ifdef TC_UCRT
        int (__cdecl *oe)(void (__cdecl *)(void)) = (void *)tc_crt_proc("_crt_atexit");  /* _crt_atexit takes a void(void) callback; used on ucrt */
        if (!oe) return 78;
        oe(hv);
#else
        int (__cdecl *(__cdecl *oe)(int (__cdecl *)(void)))(void) = (void *)tc_crt_proc("_onexit");
        if (!oe) return 78;
        oe(hx);
#endif
        exit(0); return 77;
    }
    TC_RUN(abort_in_handler);
    return tc_end();
}
