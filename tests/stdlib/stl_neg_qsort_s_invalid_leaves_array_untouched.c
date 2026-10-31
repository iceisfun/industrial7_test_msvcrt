/* TC
 * id:    stl_neg_qsort_s_invalid_leaves_array_untouched
 * area:  stdlib
 * kind:  negative
 * funcs:
 * procs: qsort_s
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs qsort_s "Parameter validation": EINVAL + invalid parameter handler; context is only passed to compare
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef int (__cdecl *cmps_t)(void *, const void *, const void *);
TC_DECL_PROC(void, qsort_s, (void *, size_t, size_t, cmps_t, void *));
static int calls;
static int __cdecl cmp(void *ctx, const void *a, const void *b) { (void)ctx; calls++; return *(const int *)a - *(const int *)b; }
static const int orig[5] = { 5, 3, 4, 1, 2 };
static int ctxv;

TC_CASE(zero_width)
{
    int a[5]; volatile size_t w = 0;
    if (!tc_p_qsort_s) TC_SKIP("no qsort_s");
    memcpy(a, orig, sizeof a); tc_iph_count = 0; errno = 0; calls = 0;
    tc_p_qsort_s(a, 5, w, cmp, &ctxv);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(calls, 0); CHECK_MEM(a, orig, sizeof a);
}
TC_CASE(null_compare)
{
    int a[5]; cmps_t volatile c = NULL;
    if (!tc_p_qsort_s) TC_SKIP("no qsort_s");
    memcpy(a, orig, sizeof a); tc_iph_count = 0; errno = 0; calls = 0;
    tc_p_qsort_s(a, 5, 4, c, &ctxv);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(calls, 0); CHECK_MEM(a, orig, sizeof a);
}
TC_CASE(null_base)
{
    void *volatile p = NULL;
    if (!tc_p_qsort_s) TC_SKIP("no qsort_s");
    tc_iph_count = 0; errno = 0; calls = 0;
    tc_p_qsort_s(p, 5, 4, cmp, &ctxv);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(calls, 0);
}
TC_CASE(null_context_is_valid)
{
    int a[5]; static const int sorted[5] = { 1, 2, 3, 4, 5 };
    void *volatile ctx = NULL;
    if (!tc_p_qsort_s) TC_SKIP("no qsort_s");
    memcpy(a, orig, sizeof a); tc_iph_count = 0; errno = 0;
    tc_p_qsort_s(a, 5, 4, cmp, ctx);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); CHECK_MEM(a, sorted, sizeof a);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(qsort_s);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph api\n"); return 1; }
    TC_RUN(zero_width); TC_RUN(null_compare); TC_RUN(null_base); TC_RUN(null_context_is_valid);
    return tc_end();
}
