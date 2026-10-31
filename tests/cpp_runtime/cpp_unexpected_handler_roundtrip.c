/* TC
 * id:    cpp_unexpected_handler_roundtrip
 * area:  cpp_runtime
 * funcs:
 * procs: set_unexpected ?set_unexpected@@YAP6AXXZP6AXXZ@Z _get_unexpected
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  documented
 * ref:   MS docs set_unexpected, _get_unexpected
 * note:  Deprecated since C++11 but still exported. Handlers are stored and compared, never invoked.
 */
#include "tc.h"

typedef void (__cdecl *th_t)(void);
typedef th_t (__cdecl *setth_t)(th_t);
typedef th_t (__cdecl *getth_t)(void);
#define N_SET "set_unexpected"
#define N_SETD "?set_unexpected@@YAP6AXXZP6AXXZ@Z"
#ifdef TC_UCRT
#define PICK(a, b) tc_crt_proc(a) ? tc_crt_proc(a) : tc_crt_proc(b)
#else
#define PICK(a, b) tc_crt_proc(b)
#endif
static setth_t p_set;
static getth_t p_get;
static th_t initial;
static void __cdecl h1(void) {}
static void __cdecl h2(void) {}

TC_CASE(set_returns_previous)
{
    if (!p_set || !p_get) TC_SKIP("set_unexpected/_get_unexpected not exported");
    initial = p_get();
    CHECK(p_set(h1) == initial);
    CHECK(p_set(h2) == h1);
}
TC_CASE(get_current)
{
    if (!p_get) TC_SKIP("_get_unexpected not exported");
    CHECK(p_get() == h2);
}
TC_CASE(set_null)
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
    p_set = (setth_t)(PICK(N_SET, N_SETD));
    p_get = (getth_t)tc_crt_proc("_get_unexpected");
    TC_RUN(set_returns_previous); TC_RUN(get_current); TC_RUN(set_null); TC_RUN(restore);
    return tc_end();
}
