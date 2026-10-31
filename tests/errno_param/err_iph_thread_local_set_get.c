/* TC
 * id:    err_iph_thread_local_set_get
 * area:  errno_param
 * funcs:
 * procs: _set_thread_local_invalid_parameter_handler _get_thread_local_invalid_parameter_handler
 * crt:   >=140
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_thread_local_invalid_parameter_handler, _get_thread_local_invalid_parameter_handler
 */
#include "tc.h"

typedef void (__cdecl *ih_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
typedef ih_t (__cdecl *set_t)(ih_t);
typedef ih_t (__cdecl *get_t)(void);
static set_t p_set; static get_t p_get;

static void __cdecl h(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e;(void)f;(void)fl;(void)l;(void)r; }

TC_CASE(exported) { CHECK(p_set != NULL); CHECK(p_get != NULL); }
TC_CASE(set_returns_previous)
{
    ih_t orig;
    if (!p_set || !p_get) TC_SKIP("missing");
    orig = p_get();
    CHECK(p_set(h) == orig);
    CHECK(p_get() == h);
    CHECK(p_set(orig) == h);
    CHECK(p_get() == orig);
}
TC_CASE(clear)
{
    if (!p_set || !p_get) TC_SKIP("missing");
    p_set(h); p_set(NULL);
    CHECK(p_get() == NULL);
}
int main(void)
{
    tc_begin();
    p_set = (set_t)tc_crt_proc("_set_thread_local_invalid_parameter_handler");
    p_get = (get_t)tc_crt_proc("_get_thread_local_invalid_parameter_handler");
    TC_RUN(exported); TC_RUN(set_returns_previous); TC_RUN(clear);
    return tc_end();
}
