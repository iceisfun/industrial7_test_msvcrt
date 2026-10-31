/* TC
 * id:    exp_ucrt_o_malloc_functional
 * area:  exports
 * funcs: 
 * procs: _o_malloc _o_calloc _o_realloc _o_free _o__msize
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   ucrtbase _o_ aliases (undocumented)
 * note:  PROBE: the _o_ heap aliases behave like malloc/calloc/realloc/free/_msize. Undocumented, assumed behavioural duplicates.
 */
#include "tc.h"


typedef void *(__cdecl *malloc_t)(size_t);
typedef void *(__cdecl *calloc_t)(size_t, size_t);
typedef void *(__cdecl *realloc_t)(void *, size_t);
typedef void  (__cdecl *free_t)(void *);
typedef size_t (__cdecl *msize_t)(void *);
static malloc_t p_malloc; static calloc_t p_calloc; static realloc_t p_realloc; static free_t p_free; static msize_t p_msize;

TC_CASE(malloc_write_free)
{
    unsigned char *p; int i;
    if (!p_malloc || !p_free) TC_SKIP("not exported");
    p = (unsigned char *)p_malloc(100);
    CHECK(p != NULL);
    if (p) { for (i = 0; i < 100; i++) p[i] = (unsigned char)i; CHECK_EQ_I(p[99], 99); p_free(p); }
}
TC_CASE(calloc_zeroed)
{
    unsigned char *p; int i, bad = 0;
    if (!p_calloc || !p_free) TC_SKIP("not exported");
    p = (unsigned char *)p_calloc(16, 8);
    CHECK(p != NULL);
    if (p) { for (i = 0; i < 128; i++) bad += p[i] != 0; CHECK_EQ_I(bad, 0); p_free(p); }
}
TC_CASE(realloc_preserves)
{
    unsigned char *p, *q; int i, bad = 0;
    if (!p_malloc || !p_realloc || !p_free) TC_SKIP("not exported");
    p = (unsigned char *)p_malloc(32);
    CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < 32; i++) p[i] = (unsigned char)(i + 1);
    q = (unsigned char *)p_realloc(p, 4096);
    CHECK(q != NULL);
    if (q) { for (i = 0; i < 32; i++) bad += q[i] != (unsigned char)(i + 1); CHECK_EQ_I(bad, 0); p_free(q); }
    else p_free(p);
}
TC_CASE(msize_at_least_requested)
{
    void *p;
    if (!p_malloc || !p_msize || !p_free) TC_SKIP("not exported");
    p = p_malloc(77);
    CHECK(p != NULL);
    if (p) { CHECK(p_msize(p) >= 77); p_free(p); }
}
int main(void)
{
    tc_begin();
    p_malloc = (malloc_t)tc_crt_proc("_o_malloc"); p_calloc = (calloc_t)tc_crt_proc("_o_calloc");
    p_realloc = (realloc_t)tc_crt_proc("_o_realloc"); p_free = (free_t)tc_crt_proc("_o_free");
    p_msize = (msize_t)tc_crt_proc("_o__msize");
    TC_RUN(malloc_write_free); TC_RUN(calloc_zeroed); TC_RUN(realloc_preserves); TC_RUN(msize_at_least_requested);
    return tc_end();
}
