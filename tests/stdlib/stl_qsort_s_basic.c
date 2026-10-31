/* TC
 * id:    stl_qsort_s_basic
 * area:  stdlib
 * funcs:
 * procs: qsort_s
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs qsort_s (comparator is int (*)(void *context, const void *, const void *))
 * note:  msvcr80/90 exports of qsort_s excluded (not certain). Comparator takes the context FIRST, unlike glibc qsort_r.
 */
#include "tc.h"
#include <stdlib.h>

TC_DECL_PROC(void, qsort_s, (void *, size_t, size_t, int (__cdecl *)(void *, const void *, const void *), void *));

static int calls;
static void *seen_ctx;
static int __cdecl cmp_dir(void *ctx, const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b, d = (x > y) - (x < y);
    calls++; seen_ctx = ctx;
    return (*(int *)ctx) ? -d : d;
}

TC_CASE(exported) { CHECK(tc_p_qsort_s != NULL); }
TC_CASE(ascending)
{
    int a[8] = { 5, 3, 9, 1, 7, 2, 8, 0 }, dir = 0;
    if (!tc_p_qsort_s) TC_SKIP("no qsort_s");
    tc_p_qsort_s(a, 8, sizeof a[0], cmp_dir, &dir);
    CHECK_EQ_I(a[0], 0); CHECK_EQ_I(a[1], 1); CHECK_EQ_I(a[2], 2); CHECK_EQ_I(a[3], 3);
    CHECK_EQ_I(a[4], 5); CHECK_EQ_I(a[5], 7); CHECK_EQ_I(a[6], 8); CHECK_EQ_I(a[7], 9);
}
TC_CASE(context_direction)
{
    int a[6] = { 4, 6, 1, 5, 2, 3 }, dir = 1, i;
    if (!tc_p_qsort_s) TC_SKIP("no qsort_s");
    tc_p_qsort_s(a, 6, sizeof a[0], cmp_dir, &dir);
    for (i = 0; i < 6; i++) CHECK_EQ_I(a[i], 6 - i);
    dir = 0;
    tc_p_qsort_s(a, 6, sizeof a[0], cmp_dir, &dir);
    for (i = 0; i < 6; i++) CHECK_EQ_I(a[i], i + 1);
}
TC_CASE(context_pointer_unchanged)
{
    int a[3] = { 3, 1, 2 }, dir = 0;
    if (!tc_p_qsort_s) TC_SKIP("no qsort_s");
    calls = 0; seen_ctx = NULL;
    tc_p_qsort_s(a, 3, sizeof a[0], cmp_dir, &dir);
    CHECK(calls > 0);
    CHECK_EQ_P(seen_ctx, &dir);
}
TC_CASE(zero_elements)
{
    int a[1] = { 42 }, dir = 0;
    if (!tc_p_qsort_s) TC_SKIP("no qsort_s");
    calls = 0;
    tc_p_qsort_s(a, 0, sizeof a[0], cmp_dir, &dir);
    CHECK_EQ_I(calls, 0); CHECK_EQ_I(a[0], 42);
    tc_p_qsort_s(a, 1, sizeof a[0], cmp_dir, &dir);
    CHECK_EQ_I(calls, 0); CHECK_EQ_I(a[0], 42);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(qsort_s);
    TC_RUN(exported); TC_RUN(ascending); TC_RUN(context_direction); TC_RUN(context_pointer_unchanged); TC_RUN(zero_elements);
    return tc_end();
}
