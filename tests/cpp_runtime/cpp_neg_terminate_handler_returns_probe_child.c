/* TC

 * id:    cpp_neg_terminate_handler_returns_probe_child
 * area:  cpp_runtime
 * kind:  negative
 * funcs:
 * procs: terminate ?terminate@@YAXXZ set_terminate ?set_terminate@@YAP6AXXZP6AXXZ@Z
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   C++ [terminate.handler] (handler shall not return); MS docs terminate, set_terminate
 * note:  PROBE: behaviour when a terminate handler returns. Expected: terminate() never returns to its caller and the library
 *        then calls abort(), which (abort message box / Watson disabled by tc_begin) exits with code 3.
 *        Plain names on ucrtbase, decorated C++ names on the older DLLs.
 *        Win10 22H2 19045 (observed): ucrtbase exports the plain names set_terminate, terminate, _get_terminate, unexpected, set_unexpected (and not
 *        the decorated C++ names). The harness macro TC_UCRT is never defined by tools/tc.py, so the plain-name branch was dead; the test now selects it with
 *        TC_AT_LEAST(140) (class F).
 */
#include "tc.h"
#include <errno.h>
#if TC_AT_LEAST(140)   /* ucrtbase; NB tools/tc.py never defines TC_UCRT */
#define N_TERM "terminate"
#define N_SETTERM "set_terminate"
#define N_UNEXP "unexpected"
#define N_SETUNEXP "set_unexpected"
#define N_SETSE "_set_se_translator"
#else
#define N_TERM "?terminate@@YAXXZ"
#define N_SETTERM "?set_terminate@@YAP6AXXZP6AXXZ@Z"
#define N_UNEXP "?unexpected@@YAXXZ"
#define N_SETUNEXP "?set_unexpected@@YAP6AXXZP6AXXZ@Z"
#define N_SETSE "?_set_se_translator@@YAP6AXIPEAU_EXCEPTION_POINTERS@@@ZP6AXI0@Z@Z"
#endif
typedef void (__cdecl *vfn)(void);
typedef vfn (__cdecl *setfn_t)(vfn);

TC_CASE(returning_handler_does_not_return)
{
    DWORD code = 0; int ran = tc_spawn_self("ret", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);   /* child's "terminate() returned" marker */
    CHECK(code != 0);
}
TC_CASE(returning_handler_aborts)
{
    DWORD code = 0; int ran = tc_spawn_self("ret", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 3);
}

static void __cdecl handler(void) { /* returns */ }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        setfn_t st = (setfn_t)tc_crt_proc(N_SETTERM);
        vfn term = (vfn)tc_crt_proc(N_TERM);
        if (!st || !term) return 78;
        st(handler);
        term();
        return 77;
    }
    TC_RUN(returning_handler_does_not_return); TC_RUN(returning_handler_aborts);
    return tc_end();
}
