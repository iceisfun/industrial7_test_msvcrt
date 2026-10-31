/* TC
 * id:    stl_rand_seed_basic
 * area:  stdlib
 * funcs: srand rand
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs rand ("Calling rand before any call to srand generates the same sequence as srand(1)"; range 0..RAND_MAX); C11 7.22.2
 * note:  The default-sequence case must run first, before any srand call in this process.
 */
#include "tc.h"
#include <stdlib.h>

static int first[6];

TC_CASE(default_is_seed1)
{
    int i, k[6];
    for (i = 0; i < 6; i++) first[i] = rand();
    srand(1);
    for (i = 0; i < 6; i++) k[i] = rand();
    for (i = 0; i < 6; i++) CHECK_EQ_I(first[i], k[i]);
}
TC_CASE(reseed_repeats)
{
    int a[8], b[8], i;
    srand(12345); for (i = 0; i < 8; i++) a[i] = rand();
    srand(12345); for (i = 0; i < 8; i++) b[i] = rand();
    for (i = 0; i < 8; i++) CHECK_EQ_I(a[i], b[i]);
}
TC_CASE(range)
{
    int i, bad = 0, mx = 0; srand(7);
    CHECK_EQ_I(RAND_MAX, 32767);
    for (i = 0; i < 5000; i++) { int r = rand(); if (r < 0 || r > RAND_MAX) bad++; if (r > mx) mx = r; }
    CHECK_EQ_I(bad, 0);
    CHECK(mx > 20000);
}
TC_CASE(different_seeds)
{
    int a, b; srand(1); a = rand(); srand(2); b = rand();
    CHECK(a != b);
}
int main(void) { tc_begin(); TC_RUN(default_is_seed1); TC_RUN(reseed_repeats); TC_RUN(range); TC_RUN(different_seeds); return tc_end(); }
