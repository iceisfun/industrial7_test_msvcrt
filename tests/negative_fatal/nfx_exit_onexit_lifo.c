/* TC
 * id:    nfx_exit_onexit_lifo
 * area:  negative_fatal
 * kind:  positive
 * funcs:  exit _exit
 * procs: _onexit _crt_atexit
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _onexit (LIFO; returns the function pointer); cf. C11 7.22.4.2 for atexit
 * note:  Handlers registered 1,2,3 run 3,2,1; state = ((0*10+3)*10+2)*10+1 = 321.
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

TC_DECL_PROC(_onexit_t, _onexit, (_onexit_t));
#if TC_CRT_VER >= 140
/* ucrtbase does NOT export _onexit (class D: the old GetProcAddress("_onexit") gave NULL and every call crashed with 0xC0000005);
 * the toolchain's static _onexit registers through the exported _crt_atexit. Older CRTs still export it. */
#define TC_ONEXIT(f) _onexit(f)
#else
#define TC_ONEXIT(f) tc_p__onexit(f)
#endif


static volatile int state;
static int __cdecl h1(void) { state = state * 10 + 1; _exit(state); return 0; }
static int __cdecl h2(void) { state = state * 10 + 2; return 0; }
static int __cdecl h3(void) { state = state * 10 + 3; return 0; }

TC_CASE(lifo_order)
{
    DWORD code = 0; int ran = tc_spawn_self("lifo", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 321);
}
TC_CASE(onexit_returns_function)
{
    DWORD code = 0; int ran = tc_spawn_self("ret", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0);
}
int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(_onexit);   /* NULL on ucrtbase, unused there */
    if ((c = tc_child_name()) != NULL) {
        _onexit_t r1 = TC_ONEXIT(h1), r2 = TC_ONEXIT(h2), r3 = TC_ONEXIT(h3);
        if (c[0] == 'r') {
            if (r1 != (_onexit_t)h1) _exit(1);
            if (r2 != (_onexit_t)h2) _exit(2);
            if (r3 != (_onexit_t)h3) _exit(3);
            _exit(0);
        }
        exit(0);
    }
    TC_RUN(lifo_order); TC_RUN(onexit_returns_function);
    return tc_end();
}
