/* TC
 * id:    err_iph_thread_local_precedence
 * area:  errno_param
 * funcs: strcpy_s _set_invalid_parameter_handler
 * procs: _set_thread_local_invalid_parameter_handler
 * crt:   >=140
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_thread_local_invalid_parameter_handler (thread-local handler used instead of the global one)
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>

typedef void (__cdecl *ih_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
typedef ih_t (__cdecl *set_t)(ih_t);
static set_t p_set;
static volatile int cg, cl;
static void __cdecl hg(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e;(void)f;(void)fl;(void)l;(void)r; cg++; }
static void __cdecl hl(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e;(void)f;(void)fl;(void)l;(void)r; cl++; }
static void bad(void) { char d[4]; volatile size_t z = 0; strcpy_s(d, z, "a"); }

TC_CASE(local_wins)
{
    if (!p_set) TC_SKIP("missing");
    cg = cl = 0;
    p_set(hl);
    bad();
    CHECK_EQ_I(cl, 1); CHECK_EQ_I(cg, 0);
}
TC_CASE(cleared_global_fires)
{
    if (!p_set) TC_SKIP("missing");
    cg = cl = 0;
    p_set(NULL);
    bad();
    CHECK_EQ_I(cg, 1); CHECK_EQ_I(cl, 0);
}
TC_CASE(null_local_falls_back)
{
    if (!p_set) TC_SKIP("missing");
    cg = cl = 0;
    p_set(hl); p_set(NULL);
    bad(); bad();
    CHECK_EQ_I(cg, 2); CHECK_EQ_I(cl, 0);
}
int main(void)
{
    tc_begin();
    p_set = (set_t)tc_crt_proc("_set_thread_local_invalid_parameter_handler");
    _set_invalid_parameter_handler(hg);
    TC_RUN(local_wins); TC_RUN(cleared_global_fires); TC_RUN(null_local_falls_back);
    return tc_end();
}
