/* TC
 * id:    stl_neg_lfind_s_invalid
 * area:  stdlib
 * kind:  negative
 * funcs:
 * procs: _lfind_s _lsearch_s
 * crt:   >=100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _lfind_s, _lsearch_s (parameter validation: EINVAL, invalid parameter handler, return NULL)
 * note:  Each invalid argument must return NULL, set errno to EINVAL and invoke the invalid-parameter handler once.
 *        A positive control case first proves the functions work with valid arguments.
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *cmp_t)(void *, const void *, const void *);
typedef void *(__cdecl *find_t)(const void *, const void *, unsigned int *, size_t, cmp_t, void *);
static find_t p_lfind_s, p_lsearch_s;

static int __cdecl cmp_int(void *ctx, const void *a, const void *b)
{ (void)ctx; return *(const int *)a != *(const int *)b; }

static int arr[4] = { 10, 20, 30, 40 };

#define EXPECT_BAD(call) do { void *r_; tc_iph_count = 0; errno = 0; r_ = (call); \
    CHECK(r_ == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(control_valid)
{
    unsigned int n = 4; volatile int key = 30; int ins = 50; int a2[5] = { 1, 2, 3, 4, 0 };
    if (!p_lfind_s) TC_SKIP("not exported");
    tc_iph_count = 0;
    CHECK(p_lfind_s((const void *)&key, arr, &n, sizeof(int), cmp_int, NULL) == &arr[2]);
    key = 99;
    CHECK(p_lfind_s((const void *)&key, arr, &n, sizeof(int), cmp_int, NULL) == NULL);
    n = 4;
    CHECK(p_lsearch_s(&ins, a2, &n, sizeof(int), cmp_int, NULL) == &a2[4]);
    CHECK_EQ_U(n, 5); CHECK_EQ_I(a2[4], 50);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(null_key)
{
    unsigned int n = 4;
    if (!p_lfind_s) TC_SKIP("not exported");
    EXPECT_BAD(p_lfind_s(NULL, arr, &n, sizeof(int), cmp_int, NULL));
    EXPECT_BAD(p_lsearch_s(NULL, arr, &n, sizeof(int), cmp_int, NULL));
    CHECK_EQ_U(n, 4);
}
TC_CASE(null_base_count_positive)
{
    unsigned int n = 3; volatile int key = 1;
    if (!p_lfind_s) TC_SKIP("not exported");
    EXPECT_BAD(p_lfind_s((const void *)&key, NULL, &n, sizeof(int), cmp_int, NULL));
    EXPECT_BAD(p_lsearch_s((const void *)&key, NULL, &n, sizeof(int), cmp_int, NULL));
    CHECK_EQ_U(n, 3);
}
TC_CASE(null_num)
{
    volatile int key = 10;
    if (!p_lfind_s) TC_SKIP("not exported");
    EXPECT_BAD(p_lfind_s((const void *)&key, arr, NULL, sizeof(int), cmp_int, NULL));
    EXPECT_BAD(p_lsearch_s((const void *)&key, arr, NULL, sizeof(int), cmp_int, NULL));
}
TC_CASE(zero_width)
{
    unsigned int n = 4; volatile int key = 10;
    if (!p_lfind_s) TC_SKIP("not exported");
    EXPECT_BAD(p_lfind_s((const void *)&key, arr, &n, 0, cmp_int, NULL));
    EXPECT_BAD(p_lsearch_s((const void *)&key, arr, &n, 0, cmp_int, NULL));
    CHECK_EQ_U(n, 4);
}
TC_CASE(null_compare)
{
    unsigned int n = 4; volatile int key = 10;
    if (!p_lfind_s) TC_SKIP("not exported");
    EXPECT_BAD(p_lfind_s((const void *)&key, arr, &n, sizeof(int), NULL, NULL));
    EXPECT_BAD(p_lsearch_s((const void *)&key, arr, &n, sizeof(int), NULL, NULL));
    CHECK_EQ_U(n, 4);
}

int main(void)
{
    tc_begin();
    p_lfind_s = (find_t)tc_crt_proc("_lfind_s");
    p_lsearch_s = (find_t)tc_crt_proc("_lsearch_s");
    CHECK(p_lfind_s && p_lsearch_s);
    CHECK(tc_iph_install());
    TC_RUN(control_valid); TC_RUN(null_key); TC_RUN(null_base_count_positive);
    TC_RUN(null_num); TC_RUN(zero_width); TC_RUN(null_compare);
    return tc_end();
}
