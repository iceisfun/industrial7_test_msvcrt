/* TC
 * id:    stl_qsort_structs_strings
 * area:  stdlib
 * funcs: qsort
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.5.2; MS docs qsort
 * note:  Comparator-argument locality (pointers into the array) is NOT asserted: the standard does not require it.
 *        See stl_qsort_inplace_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

static const char *lo, *hi;
static int outside;
static void track(const void *a, const void *b) { if ((const char *)a < lo || (const char *)a >= hi || (const char *)b < lo || (const char *)b >= hi) outside++; }

static int __cdecl cmp_byte(const void *a, const void *b) { track(a, b); return (int)*(const unsigned char *)a - (int)*(const unsigned char *)b; }
typedef struct { unsigned char v[3]; } s3;
static int __cdecl cmp_s3(const void *a, const void *b) { track(a, b); return memcmp(a, b, 3); }
typedef struct { double key; int id; int pad; double extra; } s24;
static int __cdecl cmp_s24(const void *a, const void *b) { double x = ((const s24 *)a)->key, y = ((const s24 *)b)->key; track(a, b); return (x > y) - (x < y); }
static int __cdecl cmp_str(const void *a, const void *b) { track(a, b); return strcmp(*(const char *const *)a, *(const char *const *)b); }
static int __cdecl cmp_ll(const void *a, const void *b) { long long x = *(const long long *)a, y = *(const long long *)b; track(a, b); return (x > y) - (x < y); }
typedef struct { int key; unsigned char fill[1020]; } big;
static int __cdecl cmp_big(const void *a, const void *b) { int x = ((const big *)a)->key, y = ((const big *)b)->key; track(a, b); return (x > y) - (x < y); }
#define RANGE(arr) do { lo = (const char *)(arr); hi = lo + sizeof(arr); outside = 0; } while (0)

TC_CASE(size1) { unsigned char a[] = {9, 3, 200, 0, 3, 255, 1}; RANGE(a); qsort(a, sizeof a, 1, cmp_byte); CHECK_EQ_I(a[0], 0); CHECK_EQ_I(a[1], 1); CHECK_EQ_I(a[2], 3); CHECK_EQ_I(a[3], 3); CHECK_EQ_I(a[4], 9); CHECK_EQ_I(a[5], 200); CHECK_EQ_I(a[6], 255); }
TC_CASE(size3)
{
    s3 a[4] = {{{3,2,1}}, {{1,2,3}}, {{2,2,2}}, {{1,2,2}}};
    RANGE(a); qsort(a, 4, sizeof(s3), cmp_s3);
    CHECK_EQ_I(a[0].v[2], 2); CHECK_EQ_I(a[1].v[2], 3); CHECK_EQ_I(a[2].v[0], 2); CHECK_EQ_I(a[3].v[0], 3);
}
TC_CASE(size24)
{
    s24 a[5]; int i; static const double k[5] = {2.5, -1.0, 9.75, 0.0, -7.5};
    for (i = 0; i < 5; i++) { a[i].key = k[i]; a[i].id = i; a[i].pad = 0; a[i].extra = k[i] * 2; }
    CHECK_EQ_U(sizeof(s24), 24);
    RANGE(a); qsort(a, 5, sizeof(s24), cmp_s24);
    CHECK(a[0].key == -7.5 && a[0].id == 4); CHECK(a[1].key == -1.0 && a[1].id == 1); CHECK(a[2].key == 0.0 && a[2].id == 3);
    CHECK(a[3].key == 2.5 && a[3].id == 0); CHECK(a[4].key == 9.75 && a[4].id == 2);
    for (i = 0; i < 5; i++) CHECK(a[i].extra == a[i].key * 2);
   
}
TC_CASE(strings)
{
    const char *a[] = {"pear", "apple", "fig", "banana", "apple2", "", "Zed"};
    RANGE(a); qsort(a, 7, sizeof a[0], cmp_str);
    CHECK_STR(a[0], ""); CHECK_STR(a[1], "Zed"); CHECK_STR(a[2], "apple"); CHECK_STR(a[3], "apple2");
    CHECK_STR(a[4], "banana"); CHECK_STR(a[5], "fig"); CHECK_STR(a[6], "pear");
}
TC_CASE(longlong)
{
    long long a[] = { 5, -1, -9000000000LL, 9000000000LL, 0, -5 };
    RANGE(a); qsort(a, 6, sizeof a[0], cmp_ll);
    CHECK(a[0] == -9000000000LL && a[1] == -5 && a[2] == -1 && a[3] == 0 && a[4] == 5 && a[5] == 9000000000LL);
}
TC_CASE(big_elements)
{
    static big a[6]; int i, j, bad = 0; static const int k[6] = {4, 1, 5, 0, 3, 2};
    CHECK_EQ_U(sizeof(big), 1024);
    for (i = 0; i < 6; i++) { a[i].key = k[i]; for (j = 0; j < 1020; j++) a[i].fill[j] = (unsigned char)(k[i] * 37 + j); }
    RANGE(a); qsort(a, 6, sizeof(big), cmp_big);
    for (i = 0; i < 6; i++) { if (a[i].key != i) bad++; for (j = 0; j < 1020; j++) if (a[i].fill[j] != (unsigned char)(i * 37 + j)) bad++; }
    CHECK_EQ_I(bad, 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(size1); TC_RUN(size3); TC_RUN(size24); TC_RUN(strings); TC_RUN(longlong); TC_RUN(big_elements);
    return tc_end();
}
