/* TC
 * id:    sec_neg_bsearch_s_args
 * area:  secure_crt
 * kind:  negative
 * funcs:
 * procs: bsearch_s _errno
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs bsearch_s: parameter validation table (NULL base with num>0, size==0, NULL compare, NULL key -> EINVAL + invalid-parameter handler)
 */
#include "tc.h"
#include <stddef.h>

typedef int (__cdecl *cmp_t)(void *, const void *, const void *);
typedef void *(__cdecl *bsearch_s_t)(const void *, const void *, size_t, size_t, cmp_t, void *);
typedef int *(__cdecl *errno_fn)(void);
static bsearch_s_t p_b; static errno_fn p_errno;
static int calls;
static int __cdecl cmp(void *c, const void *a, const void *b) { int x = *(const int *)a, y = *(const int *)b; (void)c; calls++; return x < y ? -1 : x > y; }

TC_CASE(bsearch_null_compare_and_zero_size)
{
    static const int a[3] = { 1, 2, 3 }; volatile int k = 2; void *r;
    *p_errno() = 0; tc_iph_count = 0;
    r = p_b((const void *)&k, a, 3, sizeof a[0], NULL, NULL);
    CHECK(r == NULL); CHECK_EQ_I(*p_errno(), 22); CHECK(tc_iph_count >= 1);
    *p_errno() = 0; tc_iph_count = 0;
    r = p_b((const void *)&k, a, 3, 0, cmp, NULL);
    CHECK(r == NULL); CHECK_EQ_I(*p_errno(), 22); CHECK(tc_iph_count >= 1);
}
int main(void)
{
    tc_begin();
    p_b = (bsearch_s_t)tc_crt_proc("bsearch_s"); p_errno = (errno_fn)tc_crt_proc("_errno");
    CHECK(p_b && p_errno); if (!(p_b && p_errno)) return tc_end();
    tc_iph_install();
    TC_RUN(bsearch_null_compare_and_zero_size);
    return tc_end();
}
