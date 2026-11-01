/* TC
 * id:    exp_neg_o_malloc_huge
 * area:  exports
 * funcs:
 * procs: _o_malloc _o_calloc _o_realloc _o_free
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs malloc/calloc/realloc (NULL and ENOMEM on failure); _o_ aliases undocumented
 * note:  Windows-observed (Win10 22H2): the _o_ exports run the plain function under ucrtbase's separate OS global state (wrapped_invoke/enter_os_call), so
 *        errno and the invalid-parameter handler are the OS-state ones (_o__errno, _o__set_invalid_parameter_handler); the app-state errno/handler stay untouched.
 *        Expectations are the documented errno/handler behaviour of the underlying function, checked against the OS state, plus app-state isolation.
 */
#include "tc.h"
#include <errno.h>
#include <stdint.h>

/* _o_ exports run the normal function under ucrtbase's separate OS global state (wrapped_invoke + enter_os_call), so errno and the
 * invalid-parameter handler are the OS-state ones: read errno via _o__errno, install the counting handler via _o__set_invalid_parameter_handler.
 * The app-state errno / handler (plain errno, _set_invalid_parameter_handler) must stay untouched (isolation check). */
static int o_iph_count;
static void __cdecl o_iph(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; o_iph_count++; }
static int *o_errp(void) { int *(__cdecl *f)(void) = (int *(__cdecl *)(void))tc_crt_proc("_o__errno"); return f ? f() : NULL; }
static int o_errno(void) { int *p = o_errp(); return p ? *p : -12345; }
static int o_install(void)   /* 1 if the OS-state handler API exists; also arms the app-state counter for the isolation check */
{
    void *(__cdecl *set)(void *) = (void *(__cdecl *)(void *))tc_crt_proc("_o__set_invalid_parameter_handler");
    if (!set || !o_errp() || !tc_iph_install()) return 0;
    set((void *)o_iph); return 1;
}
static void o_reset(void) { int *p = o_errp(); o_iph_count = 0; tc_iph_count = 0; errno = 0; if (p) *p = 0; }
static void o_isolated(void) { CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }   /* app state untouched by the _o_ call */

typedef void *(__cdecl *malloc_t)(size_t);
typedef void *(__cdecl *calloc_t)(size_t, size_t);
typedef void *(__cdecl *realloc_t)(void *, size_t);
typedef void  (__cdecl *free_t)(void *);
static malloc_t p_malloc; static calloc_t p_calloc; static realloc_t p_realloc; static free_t p_free;

TC_CASE(malloc_max_null_enomem)
{
    volatile size_t n = SIZE_MAX; void *p;
    if (!p_malloc) TC_SKIP("not exported");
    if (!o_install()) TC_SKIP("no handler api");
    o_reset();
    p = p_malloc(n);
    CHECK(p == NULL);
    CHECK_EQ_I(o_errno(), ENOMEM);
    o_isolated();
    if (p && p_free) p_free(p);
}
TC_CASE(calloc_overflow_null)
{
    volatile size_t n = SIZE_MAX / 2 + 1; void *p;
    if (!p_calloc) TC_SKIP("not exported");
    p = p_calloc(n, 4);
    CHECK(p == NULL);
    if (p && p_free) p_free(p);
}
TC_CASE(realloc_huge_null_original_intact)
{
    volatile size_t n = SIZE_MAX; unsigned char *p, *q; int i, bad = 0;
    if (!p_malloc || !p_realloc || !p_free) TC_SKIP("not exported");
    p = (unsigned char *)p_malloc(16);
    CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < 16; i++) p[i] = (unsigned char)(i + 5);
    q = (unsigned char *)p_realloc(p, n);
    CHECK(q == NULL);
    if (q) { p_free(q); return; }
    for (i = 0; i < 16; i++) bad += p[i] != (unsigned char)(i + 5);
    CHECK_EQ_I(bad, 0);
    p_free(p);
}
int main(void)
{
    tc_begin();
    p_malloc = (malloc_t)tc_crt_proc("_o_malloc"); p_calloc = (calloc_t)tc_crt_proc("_o_calloc");
    p_realloc = (realloc_t)tc_crt_proc("_o_realloc"); p_free = (free_t)tc_crt_proc("_o_free");
    TC_RUN(malloc_max_null_enomem); TC_RUN(calloc_overflow_null); TC_RUN(realloc_huge_null_original_intact);
    return tc_end();
}
