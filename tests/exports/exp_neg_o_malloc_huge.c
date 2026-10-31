/* TC
 * id:    exp_neg_o_malloc_huge
 * area:  exports
 * funcs:
 * procs: _o_malloc _o_calloc _o_realloc _o_free
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs malloc/calloc/realloc (NULL and ENOMEM on failure); _o_ aliases undocumented
 * note:  kind=negative conf=probe. Probing: _o_ allocator aliases mirror malloc docs (NULL + ENOMEM) on impossible sizes.
 */
#include "tc.h"
#include <errno.h>
#include <stdint.h>

typedef void *(__cdecl *malloc_t)(size_t);
typedef void *(__cdecl *calloc_t)(size_t, size_t);
typedef void *(__cdecl *realloc_t)(void *, size_t);
typedef void  (__cdecl *free_t)(void *);
static malloc_t p_malloc; static calloc_t p_calloc; static realloc_t p_realloc; static free_t p_free;

TC_CASE(malloc_max_null_enomem)
{
    volatile size_t n = SIZE_MAX; void *p;
    if (!p_malloc) TC_SKIP("not exported");
    errno = 0;
    p = p_malloc(n);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, ENOMEM);
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
