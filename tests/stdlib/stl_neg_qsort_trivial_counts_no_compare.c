/* TC
 * id:    stl_neg_qsort_trivial_counts_no_compare
 * area:  stdlib
 * kind:  negative
 * funcs: qsort bsearch
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.5.1 bsearch, 7.22.5.2 qsort (nothing to compare for nmemb 0/1; not-found returns NULL); MS docs qsort/bsearch
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static int calls;
static int __cdecl cmp(const void *a, const void *b) { calls++; return *(const int *)a - *(const int *)b; }
static const int arr0[8] = { 10, 20, 30, 40, 50, 60, 70, 80 };

TC_CASE(qsort_zero_one)
{
    int a[8]; volatile size_t n;
    memcpy(a, arr0, sizeof a); errno = 0; tc_iph_count = 0; calls = 0;
    n = 0; qsort(a, n, 4, cmp);
    n = 1; qsort(a, n, 4, cmp);
    CHECK_EQ_I(calls, 0); CHECK_MEM(a, arr0, sizeof a); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}
TC_CASE(bsearch_zero)
{
    int key = 10; void *r; volatile size_t n = 0;
    errno = 0; tc_iph_count = 0; calls = 0;
    r = bsearch(&key, arr0, n, 4, cmp);
    CHECK(r == NULL); CHECK_EQ_I(calls, 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}
TC_CASE(bsearch_notfound)
{
    volatile int keys[3] = { 5, 85, 45 }; int i; volatile size_t n = 8;
    for (i = 0; i < 3; i++) {
        int k = keys[i]; void *r;
        errno = 0; tc_iph_count = 0; calls = 0;
        r = bsearch(&k, arr0, n, 4, cmp);
        CHECK(r == NULL); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); CHECK(calls >= 1);
    }
}
TC_CASE(bsearch_found_control)
{
    volatile int k = 50; int kk = k; int *r;
    r = (int *)bsearch(&kk, arr0, 8, 4, cmp);
    CHECK(r == &arr0[4]);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(qsort_zero_one); TC_RUN(bsearch_zero); TC_RUN(bsearch_notfound); TC_RUN(bsearch_found_control); return tc_end(); }
