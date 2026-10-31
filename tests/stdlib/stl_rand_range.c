/* TC
 * id:    stl_rand_range
 * area:  stdlib
 * funcs: rand srand
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs rand (returns 0..RAND_MAX), RAND_MAX = 0x7fff; C11 7.22.2
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

static void scan(const char *what)
{
    int i, bad = 0, same = 1, hi = 0, lo = 0, first = rand();
    (void)what;
    if (first < 0 || first > RAND_MAX) bad++;
    for (i = 1; i < 10000; i++) {
        int r = rand();
        if (r < 0 || r > RAND_MAX) bad++;
        if (r != first) same = 0;
        if (r & 0x4000) hi++; else lo++;
    }
    CHECK_EQ_I(bad, 0);
    CHECK(!same);
    CHECK(hi > 0);
    CHECK(lo > 0);
}
TC_CASE(rand_max) { CHECK_EQ_I(RAND_MAX, 0x7fff); CHECK_EQ_I(RAND_MAX, 32767); }
TC_CASE(draws) { srand(12345); scan("12345"); }
TC_CASE(seed_zero) { volatile unsigned s = 0; srand(s); scan("0"); }
TC_CASE(seed_max) { volatile unsigned s = UINT_MAX; srand(s); scan("max"); }

int main(void) { tc_begin(); TC_RUN(rand_max); TC_RUN(draws); TC_RUN(seed_zero); TC_RUN(seed_max); return tc_end(); }
