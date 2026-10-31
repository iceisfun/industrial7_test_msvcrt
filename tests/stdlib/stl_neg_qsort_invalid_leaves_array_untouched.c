/* TC
 * id:    stl_neg_qsort_invalid_leaves_array_untouched
 * area:  stdlib
 * kind:  negative
 * funcs: qsort
 * crt:   >=140
 * tier:  p0
 * conf:  documented
 * ref:   MS docs qsort "Parameter validation": invalid arguments -> invalid parameter handler, errno = EINVAL, qsort returns
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef int (__cdecl *cmp_t)(const void *, const void *);
static int calls;
static int __cdecl cmp(const void *a, const void *b) { calls++; return *(const int *)a - *(const int *)b; }
static const int orig[5] = { 5, 3, 4, 1, 2 };

TC_CASE(zero_width)
{
    int a[5]; volatile size_t w = 0;
    memcpy(a, orig, sizeof a); tc_iph_count = 0; errno = 0; calls = 0;
    qsort(a, 5, w, cmp);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(calls, 0); CHECK_MEM(a, orig, sizeof a);
}
TC_CASE(null_compare)
{
    int a[5]; cmp_t volatile c = NULL;
    memcpy(a, orig, sizeof a); tc_iph_count = 0; errno = 0; calls = 0;
    qsort(a, 5, 4, c);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(calls, 0); CHECK_MEM(a, orig, sizeof a);
}
TC_CASE(null_base)
{
    void *volatile p = NULL;
    tc_iph_count = 0; errno = 0; calls = 0;
    qsort(p, 5, 4, cmp);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(calls, 0);
}
TC_CASE(control_valid_after)
{
    int a[5]; static const int sorted[5] = { 1, 2, 3, 4, 5 };
    memcpy(a, orig, sizeof a); tc_iph_count = 0; errno = 0;
    qsort(a, 5, 4, cmp);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); CHECK_MEM(a, sorted, sizeof a); CHECK(calls > 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph api\n"); return 1; }
    TC_RUN(zero_width); TC_RUN(null_compare); TC_RUN(null_base); TC_RUN(control_valid_after);
    return tc_end();
}
