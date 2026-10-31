/* TC

 * id:    cpp_neg_unexpected_handler_invoked_child
 * area:  cpp_runtime
 * kind:  negative
 * funcs:
 * procs: unexpected ?unexpected@@YAXXZ set_unexpected ?set_unexpected@@YAP6AXXZP6AXXZ@Z
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  documented
 * ref:   MS docs unexpected, set_unexpected; C++03 [exception.unexpected]
 * note:  Handler terminates the process with a distinctive exit code so the fatal path stays in a child.
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

TC_CASE(handler_runs)
{
    DWORD code = 0; int ran = tc_spawn_self("un", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 46);
}
static void __cdecl un_handler(void) { ExitProcess(46); }

int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) {
        setfn_t su = (setfn_t)tc_crt_proc(N_SETUNEXP);
        vfn un = (vfn)tc_crt_proc(N_UNEXP);
        if (!su || !un) return 78;
        su(un_handler);
        un();
        return 77;
    }
    TC_RUN(handler_runs);
    return tc_end();
}
