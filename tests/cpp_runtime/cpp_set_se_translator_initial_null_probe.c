/* TC

 * id:    cpp_set_se_translator_initial_null_probe
 * area:  cpp_runtime
 * funcs:
 * procs: _set_se_translator ?_set_se_translator@@YAP6AXIPEAU_EXCEPTION_POINTERS@@@ZP6AXI0@Z@Z
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_se_translator
 * note:  PROBE: the very first _set_se_translator call on a fresh thread returns NULL (no translator installed by default).
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

TC_CASE(initial_null)
{
    se_fn r;
    if (!p_set) TC_SKIP("not exported");
    r = p_set(f1);
    CHECK(r == NULL);
    p_set(NULL);
}
int main(void)
{
    tc_begin();
    p_set = (se_set_t)tc_crt_proc(N_SETSE);
    TC_RUN(initial_null);
    return tc_end();
}
