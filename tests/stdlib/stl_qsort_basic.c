/* TC
 * id:    stl_qsort_basic
 * area:  stdlib
 * funcs: qsort
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.5.2; MS docs qsort
 */
#include "tc.h"
#include <stdlib.h>

static int calls;
static int __cdecl cmp(const void *a, const void *b) { int x = *(const int *)a, y = *(const int *)b; calls++; return (x > y) - (x < y); }
static int __cdecl cmp_desc(const void *a, const void *b) { return cmp(b, a); }
static int is_sorted(const int *a, size_t n, int desc) { size_t i; for (i = 1; i < n; i++) if (desc ? a[i-1] < a[i] : a[i-1] > a[i]) return 0; return 1; }

TC_CASE(already_sorted) { int a[10] = {1,2,3,4,5,6,7,8,9,10}; qsort(a, 10, sizeof a[0], cmp); CHECK(is_sorted(a, 10, 0)); CHECK_EQ_I(a[0], 1); CHECK_EQ_I(a[9], 10); }
TC_CASE(reverse) { int a[10] = {10,9,8,7,6,5,4,3,2,1}; int i; qsort(a, 10, sizeof a[0], cmp); for (i = 0; i < 10; i++) CHECK_EQ_I(a[i], i + 1); }
TC_CASE(all_equal) { int a[20], i; for (i = 0; i < 20; i++) a[i] = 7; qsort(a, 20, sizeof a[0], cmp); for (i = 0; i < 20; i++) CHECK_EQ_I(a[i], 7); }
TC_CASE(random_100)
{
    int a[100], i; unsigned x = 12345; long long sum0 = 0, sum1 = 0; unsigned xr0 = 0, xr1 = 0;
    for (i = 0; i < 100; i++) { x = x * 1103515245u + 12345u; a[i] = (int)((x >> 8) % 1000) - 500; sum0 += a[i]; xr0 ^= (unsigned)a[i] * 2654435761u; }
    qsort(a, 100, sizeof a[0], cmp);
    for (i = 0; i < 100; i++) { sum1 += a[i]; xr1 ^= (unsigned)a[i] * 2654435761u; }
    CHECK(is_sorted(a, 100, 0));
    CHECK(sum0 == sum1);
    CHECK(xr0 == xr1);
}
TC_CASE(descending) { int a[6] = {3,1,4,1,5,9}; qsort(a, 6, sizeof a[0], cmp_desc); CHECK(is_sorted(a, 6, 1)); CHECK_EQ_I(a[0], 9); CHECK_EQ_I(a[5], 1); }
TC_CASE(small_n)
{
    int a[2] = {2, 1}, b[1] = {5};
    qsort(b, 1, sizeof b[0], cmp); CHECK_EQ_I(b[0], 5);
    qsort(a, 2, sizeof a[0], cmp); CHECK_EQ_I(a[0], 1); CHECK_EQ_I(a[1], 2);
}
TC_CASE(n_zero_no_callback)
{
    int a[1] = {9}; volatile size_t n = 0;
    calls = 0; qsort(a, n, sizeof a[0], cmp);
    CHECK_EQ_I(calls, 0); CHECK_EQ_I(a[0], 9);
}
int main(void)
{
    tc_begin();
    TC_RUN(already_sorted); TC_RUN(reverse); TC_RUN(all_equal); TC_RUN(random_100);
    TC_RUN(descending); TC_RUN(small_n); TC_RUN(n_zero_no_callback);
    return tc_end();
}
