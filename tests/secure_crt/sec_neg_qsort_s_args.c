/* TC
 * id:    sec_neg_qsort_s_args
 * area:  secure_crt
 * kind:  negative
 * funcs:
 * procs: qsort_s _errno
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs qsort_s: parameter validation table (NULL base with num>0, size==0, NULL compare + invalid-parameter handler)
 */
#include "tc.h"
#include <stddef.h>

typedef int (__cdecl *cmp_t)(void *, const void *, const void *);
typedef void (__cdecl *qsort_s_t)(void *, size_t, size_t, cmp_t, void *);
typedef int *(__cdecl *errno_fn)(void);
static qsort_s_t p_q; static errno_fn p_errno;
static int calls;
static int __cdecl cmp(void *c, const void *a, const void *b) { int x = *(const int *)a, y = *(const int *)b; (void)c; calls++; return x < y ? -1 : x > y; }

TC_CASE(qsort_null_compare)
{
    int a[3] = { 3, 2, 1 };
    *p_errno() = 0; tc_iph_count = 0;
    p_q(a, 3, sizeof a[0], NULL, NULL);
    CHECK_EQ_I(*p_errno(), 22); CHECK(tc_iph_count >= 1);
    CHECK_EQ_I(a[0], 3); CHECK_EQ_I(a[1], 2); CHECK_EQ_I(a[2], 1);
}
TC_CASE(qsort_null_base_with_num)
{
    *p_errno() = 0; tc_iph_count = 0;
    p_q(NULL, 3, sizeof(int), cmp, NULL);
    CHECK_EQ_I(*p_errno(), 22); CHECK(tc_iph_count >= 1);
}
TC_CASE(qsort_zero_size)
{
    int a[3] = { 3, 2, 1 };
    *p_errno() = 0; tc_iph_count = 0; calls = 0;
    p_q(a, 3, 0, cmp, NULL);
    CHECK_EQ_I(*p_errno(), 22); CHECK(tc_iph_count >= 1); CHECK_EQ_I(calls, 0);
}
int main(void)
{
    tc_begin();
    p_q = (qsort_s_t)tc_crt_proc("qsort_s"); p_errno = (errno_fn)tc_crt_proc("_errno");
    CHECK(p_q && p_errno); if (!(p_q && p_errno)) return tc_end();
    tc_iph_install();
    TC_RUN(qsort_null_compare); TC_RUN(qsort_null_base_with_num); TC_RUN(qsort_zero_size);
    return tc_end();
}
