/* TC
 * id:    stl_neg_qsort_invalid
 * area:  stdlib
 * kind:  negative
 * funcs: qsort
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs qsort: NULL base (num>0), size == 0 or NULL compare invoke the invalid parameter handler, errno = EINVAL
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static int calls;
static int __cdecl cmp(const void *a, const void *b) { (void)a; (void)b; calls++; return 0; }

TC_CASE(null_base)
{
    void *volatile p = NULL;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0; calls = 0;
    qsort(p, 4, sizeof(int), cmp);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(calls, 0);
}
TC_CASE(null_compare)
{
    int a[3] = {3, 2, 1}; int (__cdecl *volatile c)(const void *, const void *) = NULL;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    qsort(a, 3, sizeof a[0], c);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(a[0], 3); CHECK_EQ_I(a[2], 1);
}
TC_CASE(zero_size)
{
    int a[3] = {3, 2, 1}; volatile size_t sz = 0;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0; calls = 0;
    qsort(a, 3, sz, cmp);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(calls, 0);
    CHECK_EQ_I(a[0], 3);
}
TC_CASE(null_base_zero_count_ok)
{
    void *volatile p = NULL; volatile size_t n = 0;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    qsort(p, n, sizeof(int), cmp);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}
int main(void) { tc_begin(); TC_RUN(null_base); TC_RUN(null_compare); TC_RUN(zero_size); TC_RUN(null_base_zero_count_ok); return tc_end(); }
