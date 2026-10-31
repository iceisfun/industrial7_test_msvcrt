/* TC
 * id:    stl_qsort_inplace_probe
 * area:  stdlib
 * funcs: qsort
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.22.5.2 (silent on where comparator arguments point)
 * note:  Probes that the comparator is only ever called with pointers into the array being sorted (in-place
 *        algorithm, as in the Microsoft CRT's long-standing quicksort). Not guaranteed by the standard.
 */
#include "tc.h"
#include <stdlib.h>

static const char *lo, *hi;
static int outside;
static int __cdecl cmp(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;
    if ((const char *)a < lo || (const char *)a >= hi || (const char *)b < lo || (const char *)b >= hi) outside++;
    return (x > y) - (x < y);
}
TC_CASE(args_inside_array)
{
    int a[50], i;
    for (i = 0; i < 50; i++) a[i] = (i * 17) % 23;
    lo = (const char *)a; hi = lo + sizeof a; outside = 0;
    qsort(a, 50, sizeof a[0], cmp);
    CHECK_EQ_I(outside, 0);
    for (i = 1; i < 50; i++) CHECK(a[i-1] <= a[i]);
}
int main(void) { tc_begin(); TC_RUN(args_inside_array); return tc_end(); }
