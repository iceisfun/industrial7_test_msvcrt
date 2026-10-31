/* TC
 * id:    stl_neg_qsort_s_invalid
 * area:  stdlib
 * kind:  negative
 * funcs:
 * procs: qsort_s
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs qsort_s "Parameter validation": EINVAL + invalid parameter handler
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(void, qsort_s, (void *, size_t, size_t, int (__cdecl *)(void *, const void *, const void *), void *));

static int calls;
static int __cdecl cmp(void *ctx, const void *a, const void *b) { (void)ctx; calls++; return (a && b) ? *(const int *)a - *(const int *)b : 0; }

TC_CASE(null_base)
{
    if (!tc_p_qsort_s) TC_SKIP("no qsort_s");
    tc_iph_count = 0; errno = 0; calls = 0;
    tc_p_qsort_s(NULL, 5, sizeof(int), cmp, NULL);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(calls, 0);
}
TC_CASE(zero_width)
{
    int a[3] = { 3, 2, 1 };
    if (!tc_p_qsort_s) TC_SKIP("no qsort_s");
    tc_iph_count = 0; errno = 0; calls = 0;
    tc_p_qsort_s(a, 3, 0, cmp, NULL);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(calls, 0);
    CHECK_EQ_I(a[0], 3); CHECK_EQ_I(a[1], 2); CHECK_EQ_I(a[2], 1);
}
TC_CASE(null_compare)
{
    int a[3] = { 3, 2, 1 };
    if (!tc_p_qsort_s) TC_SKIP("no qsort_s");
    tc_iph_count = 0; errno = 0;
    tc_p_qsort_s(a, 3, sizeof a[0], NULL, NULL);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(a[0], 3); CHECK_EQ_I(a[1], 2); CHECK_EQ_I(a[2], 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(qsort_s);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(null_base); TC_RUN(zero_width); TC_RUN(null_compare);
    return tc_end();
}
