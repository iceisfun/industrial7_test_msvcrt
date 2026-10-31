/* TC
 * id:    stl_lfind_lsearch_basic
 * area:  stdlib
 * funcs: _lfind _lsearch
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _lfind, _lsearch (count is unsigned int *; comparator returns 0 when equal)
 */
#include "tc.h"
#include <stdlib.h>
#include <search.h>

static int __cdecl cmp(const void *a, const void *b) { return *(const int *)a != *(const int *)b; }

TC_CASE(lfind_found)
{
    int a[4] = { 7, 8, 9, 10 }, k = 9; unsigned int n = 4;
    CHECK_EQ_P(_lfind(&k, a, &n, sizeof(int), cmp), &a[2]);
    CHECK_EQ_U(n, 4);
    k = 7;  CHECK_EQ_P(_lfind(&k, a, &n, sizeof(int), cmp), &a[0]);
    k = 10; CHECK_EQ_P(_lfind(&k, a, &n, sizeof(int), cmp), &a[3]);
}
TC_CASE(lfind_notfound)
{
    int a[4] = { 7, 8, 9, 10 }, k = 11; unsigned int n = 4;
    CHECK(_lfind(&k, a, &n, sizeof(int), cmp) == NULL);
    CHECK_EQ_U(n, 4);
    CHECK_EQ_I(a[3], 10);
}
TC_CASE(lsearch_found)
{
    int a[5] = { 1, 2, 3, 0, 0 }, k = 2; unsigned int n = 3;
    CHECK_EQ_P(_lsearch(&k, a, &n, sizeof(int), cmp), &a[1]);
    CHECK_EQ_U(n, 3);
    CHECK_EQ_I(a[3], 0);
}
TC_CASE(lsearch_append)
{
    int a[5] = { 1, 2, 3, 0, 0 }, k = 99; unsigned int n = 3;
    CHECK_EQ_P(_lsearch(&k, a, &n, sizeof(int), cmp), &a[3]);
    CHECK_EQ_U(n, 4);
    CHECK_EQ_I(a[3], 99);
    CHECK_EQ_I(a[0], 1); CHECK_EQ_I(a[2], 3);
    CHECK_EQ_P(_lsearch(&k, a, &n, sizeof(int), cmp), &a[3]);
    CHECK_EQ_U(n, 4);
}
TC_CASE(empty)
{
    int a[2] = { 0, 0 }, k = 5; unsigned int n = 0;
    CHECK(_lfind(&k, a, &n, sizeof(int), cmp) == NULL);
    CHECK_EQ_U(n, 0);
    CHECK_EQ_P(_lsearch(&k, a, &n, sizeof(int), cmp), &a[0]);
    CHECK_EQ_U(n, 1); CHECK_EQ_I(a[0], 5);
}
int main(void)
{
    tc_begin();
    TC_RUN(lfind_found); TC_RUN(lfind_notfound); TC_RUN(lsearch_found); TC_RUN(lsearch_append); TC_RUN(empty);
    return tc_end();
}
