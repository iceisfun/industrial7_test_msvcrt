/* TC
 * id:    stl_lfind_s_lsearch_s_basic
 * area:  stdlib
 * funcs:
 * procs: _lfind_s _lsearch_s
 * crt:   >=100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _lfind_s, _lsearch_s (comparator is int (*)(void *context, const void *key, const void *datum))
 */
#include "tc.h"
#include <stdlib.h>

typedef int (__cdecl *cmps_t)(void *, const void *, const void *);
TC_DECL_PROC(void *, _lfind_s, (const void *, const void *, unsigned int *, size_t, cmps_t, void *));
TC_DECL_PROC(void *, _lsearch_s, (const void *, void *, unsigned int *, size_t, cmps_t, void *));

static void *seen_ctx; static int calls;
static int __cdecl cmp(void *ctx, const void *a, const void *b) { calls++; seen_ctx = ctx; return *(const int *)a != *(const int *)b; }

TC_CASE(exported) { CHECK(tc_p__lfind_s != NULL); CHECK(tc_p__lsearch_s != NULL); }
TC_CASE(lfind_s)
{
    int a[4] = { 7, 8, 9, 10 }, k = 9, ctx = 0; unsigned int n = 4;
    if (!tc_p__lfind_s) TC_SKIP("no _lfind_s");
    calls = 0; seen_ctx = NULL;
    CHECK_EQ_P(tc_p__lfind_s(&k, a, &n, sizeof(int), cmp, &ctx), &a[2]);
    CHECK(calls > 0); CHECK_EQ_P(seen_ctx, &ctx);
    k = 11;
    CHECK(tc_p__lfind_s(&k, a, &n, sizeof(int), cmp, &ctx) == NULL);
    CHECK_EQ_U(n, 4);
}
TC_CASE(lsearch_s)
{
    int a[5] = { 1, 2, 3, 0, 0 }, k = 2, ctx = 0; unsigned int n = 3;
    if (!tc_p__lsearch_s) TC_SKIP("no _lsearch_s");
    seen_ctx = NULL;
    CHECK_EQ_P(tc_p__lsearch_s(&k, a, &n, sizeof(int), cmp, &ctx), &a[1]);
    CHECK_EQ_U(n, 3); CHECK_EQ_P(seen_ctx, &ctx);
    k = 99;
    CHECK_EQ_P(tc_p__lsearch_s(&k, a, &n, sizeof(int), cmp, &ctx), &a[3]);
    CHECK_EQ_U(n, 4); CHECK_EQ_I(a[3], 99);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_lfind_s); TC_LOAD_PROC(_lsearch_s);
    TC_RUN(exported); TC_RUN(lfind_s); TC_RUN(lsearch_s);
    return tc_end();
}
