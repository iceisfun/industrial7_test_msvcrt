/* TC
 * id:    cpp_purecall_handler_roundtrip
 * area:  cpp_runtime
 * funcs: _purecall
 * procs: _set_purecall_handler _get_purecall_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_purecall_handler, _get_purecall_handler (VS2005+; msvcr80 excluded here only because its import lib is incomplete)
 * note:  Handlers are only stored and compared, never invoked. _purecall is referenced by address only.
 */
#include "tc.h"

typedef void (__cdecl *pc_t)(void);
typedef pc_t (__cdecl *setpc_t)(pc_t);
typedef pc_t (__cdecl *getpc_t)(void);
void __cdecl _purecall(void);
static void (__cdecl *volatile keep_purecall)(void) = _purecall;
static setpc_t p_set;
static getpc_t p_get;
static pc_t initial;

static void __cdecl h1(void) {}
static void __cdecl h2(void) {}

TC_CASE(get_initial)
{
    CHECK(keep_purecall != NULL);
    if (!p_get) TC_SKIP("_get_purecall_handler not exported");
    initial = p_get();
    CHECK(initial != h1 && initial != h2);
}
TC_CASE(set_returns_previous)
{
    if (!p_set) TC_SKIP("_set_purecall_handler not exported");
    CHECK(p_set(h1) == initial);
    CHECK(p_set(h2) == h1);
}
TC_CASE(get_current)
{
    if (!p_get) TC_SKIP("_get_purecall_handler not exported");
    CHECK(p_get() == h2);
}
TC_CASE(set_null_clears)
{
    if (!p_set || !p_get) TC_SKIP("not exported");
    CHECK(p_set(NULL) == h2);
    CHECK(p_get() == NULL);
}
TC_CASE(restore)
{
    if (!p_set || !p_get) TC_SKIP("not exported");
    p_set(initial);
    CHECK(p_get() == initial);
}
int main(void)
{
    tc_begin();
    p_set = (setpc_t)tc_crt_proc("_set_purecall_handler");
    p_get = (getpc_t)tc_crt_proc("_get_purecall_handler");
    TC_RUN(get_initial); TC_RUN(set_returns_previous); TC_RUN(get_current); TC_RUN(set_null_clears); TC_RUN(restore);
    return tc_end();
}
