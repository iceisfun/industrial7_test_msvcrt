/* TC
 * id:    gsj_neg_malloc_base_huge_returns_null
 * area:  exit_signal
 * kind:  negative
 * funcs:
 * procs: _malloc_base _free_base
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs malloc: returns NULL if insufficient memory / size above _HEAP_MAXREQ
 * note:  SIZE_MAX exceeds _HEAP_MAXREQ; the other sizes fail simply because no process can commit them (x64).
 *        errno is probed in gsj_probe_neg_alloc_base_errno_enomem.
 */
#include "tc.h"

TC_DECL_PROC(void *, _malloc_base, (size_t));
TC_DECL_PROC(void, _free_base, (void *));
static volatile size_t v_n;

TC_CASE(size_max) { void *p; if (!tc_p__malloc_base) TC_SKIP("no _malloc_base"); v_n = (size_t)-1; p = tc_p__malloc_base(v_n); CHECK(p == NULL); if (p) tc_p__free_base(p); }
TC_CASE(size_max_minus_4096) { void *p; if (!tc_p__malloc_base) TC_SKIP("no _malloc_base"); v_n = (size_t)-1 - 4096; p = tc_p__malloc_base(v_n); CHECK(p == NULL); if (p) tc_p__free_base(p); }
TC_CASE(half_address_space) { void *p; if (!tc_p__malloc_base) TC_SKIP("no _malloc_base"); v_n = ((size_t)-1) >> 1; p = tc_p__malloc_base(v_n); CHECK(p == NULL); if (p) tc_p__free_base(p); }
TC_CASE(still_works_after_failures)
{
    void *p; if (!tc_p__malloc_base) TC_SKIP("no _malloc_base");
    v_n = 16; p = tc_p__malloc_base(v_n);
    CHECK(p != NULL);
    if (p) tc_p__free_base(p);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_malloc_base); TC_LOAD_PROC(_free_base);
    TC_RUN(size_max); TC_RUN(size_max_minus_4096); TC_RUN(half_address_space); TC_RUN(still_works_after_failures);
    return tc_end();
}
