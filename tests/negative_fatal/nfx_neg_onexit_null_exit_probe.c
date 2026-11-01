/* TC
 * id:    nfx_neg_onexit_null_exit_probe
 * area:  negative_fatal
 * kind:  negative
 * funcs:  exit
 * procs: _onexit _crt_atexit
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _onexit
 * note:  Observed on Win10 22H2 19045 ucrtbase: registering a NULL handler with _onexit (on ucrtbase this is the toolchain's STATIC
 *        _onexit wrapper over the exported _crt_atexit, so the subject is mingw code, not ucrtbase: conf demoted to probe) and then calling exit(5) leaves the exit status 5 (no crash). Returning invalid-parameter handler installed
 *        in the child as before.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(_onexit_t, _onexit, (_onexit_t));
#if TC_CRT_VER >= 140
/* ucrtbase does NOT export _onexit (class D: the old GetProcAddress("_onexit") gave NULL and every call crashed with 0xC0000005);
 * the toolchain's static _onexit registers through the exported _crt_atexit. Older CRTs still export it. */
#define TC_ONEXIT(f) _onexit(f)
#else
#define TC_ONEXIT(f) tc_p__onexit(f)
#endif


TC_CASE(null_then_exit)
{
    DWORD code = 0xdead; int ran = tc_spawn_self("go", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 5);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_onexit);   /* NULL on ucrtbase, unused there */
    if (tc_child_name() != NULL) {
        volatile _onexit_t f = NULL;
        tc_iph_install();
        TC_ONEXIT(f);
        exit(5);
    }
    TC_RUN(null_then_exit);
    return tc_end();
}
