/* TC

 * id:    cpp_neg_se_translator_null_no_iph
 * area:  cpp_runtime
 * kind:  negative
 * funcs:
 * procs: _set_se_translator ?_set_se_translator@@YAP6AXIPEAU_EXCEPTION_POINTERS@@@ZP6AXI0@Z@Z
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _set_se_translator (return value = previous translator; passing 0/NULL is the conventional way to uninstall; the docs do not mention validation)
 * note:  Negative in the sense that NULL looks like a bad argument but must NOT raise the invalid-parameter handler.
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

TC_CASE(null_ok)
{
    if (!p_set) TC_SKIP("not exported");
    tc_iph_count = 0; errno = 0;
    p_set(NULL);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(null_twice_ok)
{
    se_fn r;
    if (!p_set) TC_SKIP("not exported");
    tc_iph_count = 0;
    p_set(NULL);
    r = p_set(NULL);
    CHECK(r == NULL);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(errno_untouched)
{
    if (!p_set) TC_SKIP("not exported");
    errno = 0;
    p_set(NULL);
    CHECK_EQ_I(errno, 0);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    p_set = (se_set_t)tc_crt_proc(N_SETSE);
    TC_RUN(null_ok); TC_RUN(null_twice_ok); TC_RUN(errno_untouched);
    return tc_end();
}
