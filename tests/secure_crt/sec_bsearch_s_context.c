/* TC
 * id:    sec_bsearch_s_context
 * area:  secure_crt
 * funcs:
 * procs: bsearch_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs bsearch_s: returns pointer to match or NULL; number==0 is valid
 */
#include "tc.h"
#include <stddef.h>

typedef int (__cdecl *cmp_t)(void *, const void *, const void *);
typedef void *(__cdecl *bsearch_s_t)(const void *, const void *, size_t, size_t, cmp_t, void *);
static bsearch_s_t p_bsearch_s;
static int bad_ctx, calls;

static int __cdecl cmp(void *ctx, const void *k, const void *d)
{ int x = *(const int *)k, y = *(const int *)d; if (ctx != (void *)0x77) bad_ctx = 1; calls++; return x < y ? -1 : x > y; }
static int __cdecl cmp_plain(void *ctx, const void *k, const void *d)
{ int x = *(const int *)k, y = *(const int *)d; (void)ctx; return x < y ? -1 : x > y; }

static const int arr[7] = { 2, 4, 6, 8, 10, 12, 14 };

TC_CASE(found_returns_element_pointer)
{
    volatile int key = 10; void *r;
    if (!p_bsearch_s) TC_SKIP("bsearch_s not exported");
    r = p_bsearch_s((const void *)&key, arr, 7, sizeof arr[0], cmp_plain, NULL);
    CHECK_EQ_P(r, &arr[4]);
}
TC_CASE(not_found_null)
{
    volatile int key = 5; void *r;
    if (!p_bsearch_s) TC_SKIP("bsearch_s not exported");
    r = p_bsearch_s((const void *)&key, arr, 7, sizeof arr[0], cmp_plain, NULL);
    CHECK(r == NULL);
    key = 100; r = p_bsearch_s((const void *)&key, arr, 7, sizeof arr[0], cmp_plain, NULL); CHECK(r == NULL);
    key = -3;  r = p_bsearch_s((const void *)&key, arr, 7, sizeof arr[0], cmp_plain, NULL); CHECK(r == NULL);
}
TC_CASE(context_forwarded)
{
    volatile int key = 8; void *r;
    if (!p_bsearch_s) TC_SKIP("bsearch_s not exported");
    bad_ctx = 0; calls = 0;
    r = p_bsearch_s((const void *)&key, arr, 7, sizeof arr[0], cmp, (void *)0x77);
    CHECK_EQ_P(r, &arr[3]); CHECK(calls > 0); CHECK(!bad_ctx);
}
TC_CASE(first_and_last_element)
{
    volatile int key = 2; void *r;
    if (!p_bsearch_s) TC_SKIP("bsearch_s not exported");
    r = p_bsearch_s((const void *)&key, arr, 7, sizeof arr[0], cmp_plain, NULL); CHECK_EQ_P(r, &arr[0]);
    key = 14; r = p_bsearch_s((const void *)&key, arr, 7, sizeof arr[0], cmp_plain, NULL); CHECK_EQ_P(r, &arr[6]);
}
TC_CASE(num_zero_returns_null_no_error)
{
    volatile int key = 2; void *r; int e;
    if (!p_bsearch_s) TC_SKIP("bsearch_s not exported");
    tc_iph_install(); tc_iph_count = 0; calls = 0; bad_ctx = 0;
    r = p_bsearch_s((const void *)&key, arr, 0, sizeof arr[0], cmp, (void *)0x77);
    (void)e;
    CHECK(r == NULL); CHECK_EQ_I(calls, 0); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    p_bsearch_s = (bsearch_s_t)tc_crt_proc("bsearch_s");
    TC_RUN(found_returns_element_pointer); TC_RUN(not_found_null); TC_RUN(context_forwarded);
    TC_RUN(first_and_last_element); TC_RUN(num_zero_returns_null_no_error);
    return tc_end();
}
