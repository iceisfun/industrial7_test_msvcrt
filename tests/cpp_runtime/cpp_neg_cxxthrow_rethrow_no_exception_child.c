/* TC

 * id:    cpp_neg_cxxthrow_rethrow_no_exception_child
 * area:  cpp_runtime
 * kind:  negative
 * funcs: _CxxThrowException
 * procs: set_terminate ?set_terminate@@YAP6AXXZP6AXXZ@Z
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   C++ [except.throw]/9; MS docs terminate
 * note:  PROBE: (NULL, NULL) is how MSVC lowers a bare 'throw;'. _CxxThrowException itself only raises SEH 0xE06D7363; the
 *        "no exception being handled -> terminate()" decision is made by the C++ frame handler, which only runs when a C++ frame is on
 *        the stack. This plain-C child has none, so the most likely outcome is an unhandled exception that ends the process with
 *        exit code 0xE06D7363 (SEM_NOGPFAULTERRORBOX is set by tc_begin). Whether the process instead reaches terminate() is what real
 *        Windows will tell us. The terminate-handler variant only asserts that the call never returns normally.
 *        ucrtbase omitted: _CxxThrowException is exported by vcruntime140, not ucrtbase.
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

extern void __cdecl _CxxThrowException(void *, void *);
static void __cdecl term_handler(void) { ExitProcess(47); }

TC_CASE(never_returns_with_terminate_handler)
{
    DWORD code = 0; int ran = tc_spawn_self("withhandler", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);   /* child's "_CxxThrowException returned" marker */
    CHECK(code != 78);
}
TC_CASE(unhandled_msc_exception)
{
    DWORD code = 0; int ran = tc_spawn_self("default", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 0xE06D7363u);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 'w') {
            setfn_t st = (setfn_t)tc_crt_proc(N_SETTERM);
            if (!st) return 78;
            st(term_handler);
        }
        _CxxThrowException(NULL, NULL);
        return 77;
    }
    TC_RUN(never_returns_with_terminate_handler); TC_RUN(unhandled_msc_exception);
    return tc_end();
}
