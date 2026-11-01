/* TC
 * id:    nfx_exit_underscore_exit_in_atexit
 * area:  negative_fatal
 * kind:  positive
 * funcs: _exit  exit
 * procs: _onexit _crt_atexit
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _exit
 * note:  _exit inside a handler ends the process at once; earlier-registered handlers never run.
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


static int __cdecl h1(void) { _exit(77); return 0; }  /* registered first, runs last: never reached */
static int __cdecl h2(void) { _exit(8); return 0; }

TC_CASE(underscore_exit_in_handler)
{
    DWORD code = 0xdead; int ran = tc_spawn_self("go", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 8);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_onexit);   /* NULL on ucrtbase, unused there */
    if (tc_child_name() != NULL) {
        if (!TC_ONEXIT(h1) || !TC_ONEXIT(h2)) return 50;
        exit(0);
    }
    TC_RUN(underscore_exit_in_handler);
    return tc_end();
}
