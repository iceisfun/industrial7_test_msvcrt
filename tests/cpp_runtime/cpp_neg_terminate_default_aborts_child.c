/* TC
 * id:    cpp_neg_terminate_default_aborts_child
 * area:  cpp_runtime
 * funcs:
 * procs: terminate ?terminate@@YAXXZ set_terminate ?set_terminate@@YAP6AXXZP6AXXZ@Z
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs terminate (default handler calls abort), abort (exit code 3), set_terminate
 * note:  tc_begin clears _CALL_REPORTFAULT so abort exits with code 3.
 */
#include "tc.h"

typedef void (__cdecl *th_t)(void);
typedef th_t (__cdecl *setth_t)(th_t);
#define N_SET "set_terminate"
#define N_TERM "terminate"
#define N_SETD "?set_terminate@@YAP6AXXZP6AXXZ@Z"
#define N_TERMD "?terminate@@YAXXZ"
#if TC_CRT_VER >= 140
#define PICK(a, b) tc_crt_proc(a) ? tc_crt_proc(a) : tc_crt_proc(b)
#else
#define PICK(a, b) tc_crt_proc(b)
#endif
static void __cdecl h43(void) { ExitProcess(43); }

TC_CASE(default_exit_3)
{
    DWORD code = 0; int ran = tc_spawn_self("dflt", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 3);
}
TC_CASE(reset_to_default)
{
    DWORD code = 0; int ran = tc_spawn_self("reset", &code, 20000);
    CHECK(ran);
    CHECK(code != 43);
    CHECK_EQ_U(code, 3);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        setth_t set = (setth_t)(PICK(N_SET, N_SETD));
        th_t term = (th_t)(PICK(N_TERM, N_TERMD));
        if (!set || !term) return 90;
        if (c[0] == 'r') { set(h43); set(NULL); }
        term();
        return 77;
    }
    TC_RUN(default_exit_3); TC_RUN(reset_to_default);
    return tc_end();
}
