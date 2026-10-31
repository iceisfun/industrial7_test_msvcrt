/* TC

 * id:    cpp_set_se_translator_int_overload_probe
 * area:  cpp_runtime
 * funcs:
 * procs: ?_set_se_translator@@YAP6AXIPEAU_EXCEPTION_POINTERS@@@ZH@Z ?_set_se_translator@@YAP6AXIPEAU_EXCEPTION_POINTERS@@@ZP6AXI0@Z@Z
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_se_translator
 * note:  PROBE: the legacy 'ZH@Z' (int argument) overload exists for source compatibility with passing 0 and is expected to clear the
 *        translator and return the previous one.
 */
#include "tc.h"
#include <errno.h>
typedef void (__cdecl *se_fn)(unsigned int, void *);
typedef se_fn (__cdecl *se_set_t)(se_fn);
typedef se_fn (__cdecl *se_seti_t)(int);
static se_set_t p_set; static se_seti_t p_seti;
static void __cdecl f1(unsigned int c, void *p) { (void)c; (void)p; }

TC_CASE(int_zero_clears)
{
    se_fn r;
    if (!p_set || !p_seti) TC_SKIP("overload not exported");
    p_set(f1);
    r = p_seti(0);
    CHECK(r == f1);
}
TC_CASE(after_int_zero_null)
{
    se_fn r;
    if (!p_set || !p_seti) TC_SKIP("overload not exported");
    p_set(f1);
    p_seti(0);
    r = p_set(NULL);
    CHECK(r == NULL);
}
int main(void)
{
    tc_begin();
    p_set = (se_set_t)tc_crt_proc("?_set_se_translator@@YAP6AXIPEAU_EXCEPTION_POINTERS@@@ZP6AXI0@Z@Z");
    p_seti = (se_seti_t)tc_crt_proc("?_set_se_translator@@YAP6AXIPEAU_EXCEPTION_POINTERS@@@ZH@Z");
    TC_RUN(int_zero_clears); TC_RUN(after_int_zero_null);
    return tc_end();
}
