/* TC

 * id:    cpp_set_se_translator_roundtrip
 * area:  cpp_runtime
 * funcs:
 * procs: _set_se_translator ?_set_se_translator@@YAP6AXIPEAU_EXCEPTION_POINTERS@@@ZP6AXI0@Z@Z
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_se_translator (returns a pointer to the previous translator)
 * note:  The translator is never triggered (that needs /EHa). Plain name on ucrtbase, decorated otherwise.
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

typedef void (__cdecl *se_fn)(unsigned int, void *);
typedef se_fn (__cdecl *se_set_t)(se_fn);
static se_set_t p_set;
static void __cdecl f1(unsigned int c, void *p) { (void)c; (void)p; }
static void __cdecl f2(unsigned int c, void *p) { (void)c; (void)p; }

TC_CASE(set_returns_previous)
{
    se_fn r;
    if (!p_set) TC_SKIP("not exported");
    p_set(f1);
    r = p_set(f2);
    CHECK(r == f1);
}
TC_CASE(set_null_returns_prev)
{
    se_fn r;
    if (!p_set) TC_SKIP("not exported");
    p_set(f2);
    r = p_set(NULL);
    CHECK(r == f2);
}
TC_CASE(restore)
{
    se_fn r;
    if (!p_set) TC_SKIP("not exported");
    r = p_set(NULL);
    CHECK(r == NULL);
}
TC_CASE(exported) { CHECK(p_set != NULL); }

int main(void)
{
    tc_begin();
    p_set = (se_set_t)tc_crt_proc(N_SETSE);
    TC_RUN(exported); TC_RUN(set_returns_previous); TC_RUN(set_null_returns_prev); TC_RUN(restore);
    return tc_end();
}
