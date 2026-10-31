/* TC
 * id:    stl_bsearch_s_basic
 * area:  stdlib
 * funcs:
 * procs: bsearch_s
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs bsearch_s; comparator is int (*)(void *context, const void *key, const void *datum)
 */
#include "tc.h"
#include <stdlib.h>

TC_DECL_PROC(void *, bsearch_s, (const void *, const void *, size_t, size_t, int (__cdecl *)(void *, const void *, const void *), void *));

static const void *seen_key; static void *seen_ctx; static int calls;
static int __cdecl cmp(void *ctx, const void *k, const void *e)
{
    int a = *(const int *)k, b = *(const int *)e;
    calls++; seen_key = k; seen_ctx = ctx;
    return (a > b) - (a < b);
}
static int arr[5] = { 10, 20, 30, 40, 50 };

TC_CASE(exported) { CHECK(tc_p_bsearch_s != NULL); }
TC_CASE(found_notfound)
{
    int k, ctx = 0;
    if (!tc_p_bsearch_s) TC_SKIP("no bsearch_s");
    k = 10; CHECK_EQ_P(tc_p_bsearch_s(&k, arr, 5, sizeof(int), cmp, &ctx), &arr[0]);
    k = 30; CHECK_EQ_P(tc_p_bsearch_s(&k, arr, 5, sizeof(int), cmp, &ctx), &arr[2]);
    k = 50; CHECK_EQ_P(tc_p_bsearch_s(&k, arr, 5, sizeof(int), cmp, &ctx), &arr[4]);
    k = 35; CHECK(tc_p_bsearch_s(&k, arr, 5, sizeof(int), cmp, &ctx) == NULL);
    k = 5;  CHECK(tc_p_bsearch_s(&k, arr, 5, sizeof(int), cmp, &ctx) == NULL);
    k = 60; CHECK(tc_p_bsearch_s(&k, arr, 5, sizeof(int), cmp, &ctx) == NULL);
}
TC_CASE(context_and_key)
{
    int k = 40, ctx = 0;
    if (!tc_p_bsearch_s) TC_SKIP("no bsearch_s");
    calls = 0; seen_key = NULL; seen_ctx = NULL;
    CHECK_EQ_P(tc_p_bsearch_s(&k, arr, 5, sizeof(int), cmp, &ctx), &arr[3]);
    CHECK(calls > 0);
    CHECK_EQ_P(seen_ctx, &ctx);
    CHECK_EQ_P(seen_key, &k);
}
TC_CASE(empty)
{
    int k = 1, ctx = 0;
    if (!tc_p_bsearch_s) TC_SKIP("no bsearch_s");
    calls = 0;
    CHECK(tc_p_bsearch_s(&k, arr, 0, sizeof(int), cmp, &ctx) == NULL);
    CHECK_EQ_I(calls, 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(bsearch_s);
    TC_RUN(exported); TC_RUN(found_notfound); TC_RUN(context_and_key); TC_RUN(empty);
    return tc_end();
}
