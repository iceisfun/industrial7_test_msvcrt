/* TC
 * id:    stl_bsearch_basic
 * area:  stdlib
 * funcs: bsearch
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.5.1
 */
#include "tc.h"
#include <stdlib.h>

static int __cdecl cmp(const void *k, const void *e) { int a = *(const int *)k, b = *(const int *)e; return (a > b) - (a < b); }
static int arr[7] = { 2, 4, 6, 8, 10, 12, 14 };
static volatile size_t n7 = 7;

TC_CASE(found)
{
    int k;
    k = 2;  CHECK_EQ_P(bsearch(&k, arr, n7, sizeof(int), cmp), &arr[0]);
    k = 14; CHECK_EQ_P(bsearch(&k, arr, n7, sizeof(int), cmp), &arr[6]);
    k = 8;  CHECK_EQ_P(bsearch(&k, arr, n7, sizeof(int), cmp), &arr[3]);
    k = 4;  CHECK_EQ_P(bsearch(&k, arr, n7, sizeof(int), cmp), &arr[1]);
    k = 12; CHECK_EQ_P(bsearch(&k, arr, n7, sizeof(int), cmp), &arr[5]);
}
TC_CASE(not_found)
{
    int k;
    k = 1;  CHECK(bsearch(&k, arr, n7, sizeof(int), cmp) == NULL);
    k = 7;  CHECK(bsearch(&k, arr, n7, sizeof(int), cmp) == NULL);
    k = 15; CHECK(bsearch(&k, arr, n7, sizeof(int), cmp) == NULL);
}
TC_CASE(single)
{
    int one[1] = { 5 }, k;
    k = 5; CHECK_EQ_P(bsearch(&k, one, 1, sizeof(int), cmp), &one[0]);
    k = 4; CHECK(bsearch(&k, one, 1, sizeof(int), cmp) == NULL);
    k = 6; CHECK(bsearch(&k, one, 1, sizeof(int), cmp) == NULL);
}
TC_CASE(empty)
{
    int k = 3; volatile size_t z = 0;
    CHECK(bsearch(&k, arr, z, sizeof(int), cmp) == NULL);
}
TC_CASE(duplicates)
{
    int d[7] = { 1, 3, 3, 3, 3, 5, 9 }, k = 3; int *r;
    r = (int *)bsearch(&k, d, 7, sizeof(int), cmp);
    CHECK(r != NULL);
    if (r) { CHECK_EQ_I(*r, 3); CHECK(r >= &d[1] && r <= &d[4]); }
}
int main(void)
{
    tc_begin();
    TC_RUN(found); TC_RUN(not_found); TC_RUN(single); TC_RUN(empty); TC_RUN(duplicates);
    return tc_end();
}
