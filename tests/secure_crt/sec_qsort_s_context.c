/* TC
 * id:    sec_qsort_s_context
 * area:  secure_crt
 * funcs:
 * procs: qsort_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs qsort_s: int (__cdecl *compare)(void *context, const void *key, const void *datum)
 */
#include "tc.h"
#include <stddef.h>

typedef int (__cdecl *cmp_t)(void *, const void *, const void *);
typedef void (__cdecl *qsort_s_t)(void *, size_t, size_t, cmp_t, void *);
static qsort_s_t p_qsort_s;

static int calls;
static int seen_ctx_bad;
static int __cdecl cmp_asc(void *ctx, const void *a, const void *b)
{ int x = *(const int *)a, y = *(const int *)b; (void)ctx; calls++; return x < y ? -1 : x > y; }
static int __cdecl cmp_ctx(void *ctx, const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b; int desc = *(int *)ctx;
    if (ctx != (void *)&desc && desc != 0 && desc != 1) seen_ctx_bad = 1;
    calls++;
    if (desc) return x < y ? 1 : x > y ? -1 : 0;
    return x < y ? -1 : x > y;
}
static int __cdecl cmp_marker(void *ctx, const void *a, const void *b)
{ int x = *(const int *)a, y = *(const int *)b; if (ctx != (void *)0x1234) seen_ctx_bad = 1; calls++; return x < y ? -1 : x > y; }
struct rec { int key; int pad1; int pad2; };
static int __cdecl cmp_rec(void *ctx, const void *a, const void *b)
{ int x = ((const struct rec *)a)->key, y = ((const struct rec *)b)->key; (void)ctx; return x < y ? -1 : x > y; }

TC_CASE(sorts_ints_ascending)
{
    int a[8] = { 5, 3, 9, -1, 0, 7, 3, 2 }; int i;
    if (!p_qsort_s) TC_SKIP("qsort_s not exported");
    p_qsort_s(a, 8, sizeof a[0], cmp_asc, NULL);
    for (i = 1; i < 8; i++) CHECK(a[i - 1] <= a[i]);
    CHECK_EQ_I(a[0], -1); CHECK_EQ_I(a[7], 9);
}
TC_CASE(context_pointer_forwarded)
{
    int a[6] = { 4, 2, 6, 1, 5, 3 }; int i;
    if (!p_qsort_s) TC_SKIP("qsort_s not exported");
    seen_ctx_bad = 0; calls = 0;
    p_qsort_s(a, 6, sizeof a[0], cmp_marker, (void *)0x1234);
    CHECK(calls > 0); CHECK(!seen_ctx_bad);
    for (i = 0; i < 6; i++) CHECK_EQ_I(a[i], i + 1);
}
TC_CASE(reverse_sorted_input)
{
    int a[10] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 }; int i;
    if (!p_qsort_s) TC_SKIP("qsort_s not exported");
    p_qsort_s(a, 10, sizeof a[0], cmp_asc, NULL);
    for (i = 0; i < 10; i++) CHECK_EQ_I(a[i], i + 1);
}
TC_CASE(num_0_and_1_no_compare_calls)
{
    int a[2] = { 2, 1 };
    if (!p_qsort_s) TC_SKIP("qsort_s not exported");
    calls = 0;
    p_qsort_s(a, 0, sizeof a[0], cmp_asc, NULL);
    CHECK_EQ_I(calls, 0); CHECK_EQ_I(a[0], 2); CHECK_EQ_I(a[1], 1);
    p_qsort_s(a, 1, sizeof a[0], cmp_asc, NULL);
    CHECK_EQ_I(calls, 0); CHECK_EQ_I(a[0], 2); CHECK_EQ_I(a[1], 1);
}
TC_CASE(struct_elements_size_12)
{
    struct rec r[5]; int i; int keys[5] = { 50, 10, 40, 20, 30 };
    if (!p_qsort_s) TC_SKIP("qsort_s not exported");
    CHECK_EQ_U(sizeof(struct rec), 12);
    for (i = 0; i < 5; i++) { r[i].key = keys[i]; r[i].pad1 = keys[i] + 1; r[i].pad2 = keys[i] + 2; }
    p_qsort_s(r, 5, sizeof r[0], cmp_rec, NULL);
    for (i = 0; i < 5; i++) { CHECK_EQ_I(r[i].key, (i + 1) * 10); CHECK_EQ_I(r[i].pad1, (i + 1) * 10 + 1); CHECK_EQ_I(r[i].pad2, (i + 1) * 10 + 2); }
}
TC_CASE(descending_via_context_flag)
{
    int a[6] = { 3, 1, 4, 1, 5, 9 }; int flag = 1; int i;
    if (!p_qsort_s) TC_SKIP("qsort_s not exported");
    seen_ctx_bad = 0;
    p_qsort_s(a, 6, sizeof a[0], cmp_ctx, &flag);
    for (i = 1; i < 6; i++) CHECK(a[i - 1] >= a[i]);
    CHECK_EQ_I(a[0], 9);
    flag = 0;
    p_qsort_s(a, 6, sizeof a[0], cmp_ctx, &flag);
    for (i = 1; i < 6; i++) CHECK(a[i - 1] <= a[i]);
    CHECK(!seen_ctx_bad);
}
int main(void)
{
    tc_begin();
    p_qsort_s = (qsort_s_t)tc_crt_proc("qsort_s");
    TC_RUN(sorts_ints_ascending); TC_RUN(context_pointer_forwarded); TC_RUN(reverse_sorted_input);
    TC_RUN(num_0_and_1_no_compare_calls); TC_RUN(struct_elements_size_12); TC_RUN(descending_via_context_flag);
    return tc_end();
}
