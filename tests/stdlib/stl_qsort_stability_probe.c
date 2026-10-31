/* TC
 * id:    stl_qsort_stability_probe
 * area:  stdlib
 * funcs: qsort
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.22.5.2 (order of equal elements unspecified)
 * note:  Records whether qsort preserved the original order of equal keys (TC NOTE line). Asserts ONLY sortedness by key,
 *        never stability.
 */
#include "tc.h"
#include <stdlib.h>

typedef struct { int key; int tag; } rec;
static int __cdecl cmp(const void *a, const void *b) { int x = ((const rec *)a)->key, y = ((const rec *)b)->key; return (x > y) - (x < y); }

TC_CASE(record_stability)
{
    rec a[64]; int i, sorted = 1, stable = 1;
    for (i = 0; i < 64; i++) { a[i].key = (i * 7) % 5; a[i].tag = i; }
    qsort(a, 64, sizeof a[0], cmp);
    for (i = 1; i < 64; i++) {
        if (a[i-1].key > a[i].key) sorted = 0;
        if (a[i-1].key == a[i].key && a[i-1].tag > a[i].tag) stable = 0;
    }
    CHECK(sorted);
    tc_write("TC NOTE " TC_TEST_ID " " TC_CRT_DLL); tc_write(stable ? " qsort stable on this input\n" : " qsort NOT stable on this input\n");
}
int main(void) { tc_begin(); TC_RUN(record_stability); return tc_end(); }
