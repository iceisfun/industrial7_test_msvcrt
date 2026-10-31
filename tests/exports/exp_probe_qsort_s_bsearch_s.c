/* TC
 * id:    exp_probe_qsort_s_bsearch_s
 * area:  exports
 * funcs:
 * procs: qsort_s bsearch_s
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs qsort_s, bsearch_s
 * note:  Probing: whether qsort_s/bsearch_s are exported from msvcr80/msvcr90 or only from msvcr100+. Callable behaviour is textbook
 *        (context passed first to the comparator in the MS signature).
 */
#include "tc.h"

static int ctx_seen;
static int __cdecl cmp(void *ctx, const void *a, const void *b)
{
    if (ctx == &ctx_seen) ctx_seen++;
    return *(const int *)a - *(const int *)b;
}
TC_CASE(qsort_s_exported) { CHECK(tc_crt_proc("qsort_s") != NULL); }
TC_CASE(bsearch_s_exported) { CHECK(tc_crt_proc("bsearch_s") != NULL); }
TC_CASE(sort_and_search)
{
    void (__cdecl *qs)(void *, size_t, size_t, int (__cdecl *)(void *, const void *, const void *), void *) =
        (void (__cdecl *)(void *, size_t, size_t, int (__cdecl *)(void *, const void *, const void *), void *))tc_crt_proc("qsort_s");
    void *(__cdecl *bs)(const void *, const void *, size_t, size_t, int (__cdecl *)(void *, const void *, const void *), void *) =
        (void *(__cdecl *)(const void *, const void *, size_t, size_t, int (__cdecl *)(void *, const void *, const void *), void *))tc_crt_proc("bsearch_s");
    int a[6] = { 5, 3, 9, 1, 7, 2 }, key = 7; int *r;
    if (!qs || !bs) TC_SKIP("not exported");
    qs(a, 6, sizeof a[0], cmp, &ctx_seen);
    CHECK_EQ_I(a[0], 1); CHECK_EQ_I(a[1], 2); CHECK_EQ_I(a[2], 3); CHECK_EQ_I(a[3], 5); CHECK_EQ_I(a[4], 7); CHECK_EQ_I(a[5], 9);
    CHECK(ctx_seen > 0);
    r = (int *)bs(&key, a, 6, sizeof a[0], cmp, &ctx_seen);
    CHECK(r == &a[4]);
    key = 4; CHECK(bs(&key, a, 6, sizeof a[0], cmp, &ctx_seen) == NULL);
}
int main(void) { tc_begin(); TC_RUN(qsort_s_exported); TC_RUN(bsearch_s_exported); TC_RUN(sort_and_search); return tc_end(); }
