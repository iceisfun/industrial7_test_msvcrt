/* TC
 * id:    cpp_neg_terminate_calls_handler_child
 * area:  cpp_runtime
 * funcs:
 * procs: terminate ?terminate@@YAXXZ set_terminate ?set_terminate@@YAP6AXXZP6AXXZ@Z
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs terminate, set_terminate
 * note:  terminate() calls the most recently installed handler; the handler here never returns (ExitProcess).
 *        Win10 22H2 19045 (observed): ucrtbase exports the plain names set_terminate, terminate, _get_terminate, unexpected, set_unexpected (and not
 *        the decorated C++ names). The harness macro TC_UCRT is never defined by tools/tc.py, so the plain-name branch was dead; the test now selects it with
 *        TC_AT_LEAST(140) (class F).
 */
#include "tc.h"

typedef void (__cdecl *th_t)(void);
typedef th_t (__cdecl *setth_t)(th_t);
#define N_SET "set_terminate"
#define N_TERM "terminate"
#define N_SETD "?set_terminate@@YAP6AXXZP6AXXZ@Z"
#define N_TERMD "?terminate@@YAXXZ"
#if TC_AT_LEAST(140)   /* ucrtbase; NB tools/tc.py never defines TC_UCRT */
#define PICK(a, b) tc_crt_proc(a) ? tc_crt_proc(a) : tc_crt_proc(b)
#else
#define PICK(a, b) tc_crt_proc(b)
#endif
static void __cdecl h43(void) { ExitProcess(43); }
static void __cdecl h44(void) { ExitProcess(44); }

TC_CASE(handler_runs)
{
    DWORD code = 0; int ran = tc_spawn_self("one", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 43);
}
TC_CASE(handler_replaced)
{
    DWORD code = 0; int ran = tc_spawn_self("two", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 43);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        setth_t set = (setth_t)(PICK(N_SET, N_SETD));
        th_t term = (th_t)(PICK(N_TERM, N_TERMD));
        if (!set || !term) return 90;
        if (c[0] == 't') set(h44);
        set(h43);
        term();
        return 77;
    }
    TC_RUN(handler_runs); TC_RUN(handler_replaced);
    return tc_end();
}
