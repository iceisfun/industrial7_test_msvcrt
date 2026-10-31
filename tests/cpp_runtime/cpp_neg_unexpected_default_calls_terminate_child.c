/* TC

 * id:    cpp_neg_unexpected_default_calls_terminate_child
 * area:  cpp_runtime
 * kind:  negative
 * funcs:
 * procs: unexpected ?unexpected@@YAXXZ set_terminate ?set_terminate@@YAP6AXXZP6AXXZ@Z
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  standard
 * ref:   C++03 [exception.unexpected] (default unexpected_handler calls terminate); MS docs unexpected, set_terminate
 * note:  ucrtbase omitted: unexpected() was removed from the language in C++17 and the default in vcruntime140 is unverified.
 */
#include "tc.h"
#include <errno.h>
#ifdef TC_UCRT
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

TC_CASE(default_chain)
{
    DWORD code = 0; int ran = tc_spawn_self("chain", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 45);
}
static void __cdecl term_handler(void) { ExitProcess(45); }

int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) {
        setfn_t st = (setfn_t)tc_crt_proc(N_SETTERM);
        vfn un = (vfn)tc_crt_proc(N_UNEXP);
        if (!st || !un) return 78;
        st(term_handler);
        un();
        return 77;
    }
    TC_RUN(default_chain);
    return tc_end();
}
