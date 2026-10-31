/* TC
 * id:    stl_rand_srand_repeat
 * area:  stdlib
 * funcs: rand srand
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs srand / rand: same seed gives same sequence; rand before srand behaves as srand(1); C11 7.22.2.2
 * note:  default_seed_is_one must run first, before any srand call in the process.
 */
#include "tc.h"
#include <stdlib.h>

static void draw(int *a, int n) { int i; for (i = 0; i < n; i++) a[i] = rand(); }
static int same(const int *a, const int *b, int n) { int i; for (i = 0; i < n; i++) if (a[i] != b[i]) return 0; return 1; }

TC_CASE(default_seed_is_one)
{
    int a[16], b[16];
    draw(a, 16);          /* no srand yet in this process */
    srand(1);
    draw(b, 16);
    CHECK(same(a, b, 16));
}
TC_CASE(repeat)
{
    int a[16], b[16]; volatile unsigned s = 42;
    srand(s); draw(a, 16);
    srand(s); draw(b, 16);
    CHECK(same(a, b, 16));
}
TC_CASE(different_seeds_differ)
{
    int a[16], b[16]; volatile unsigned s1 = 42, s2 = 43;
    srand(s1); draw(a, 16);
    srand(s2); draw(b, 16);
    CHECK(!same(a, b, 16));
}
TC_CASE(reseed_restarts)
{
    int a[8], mid[8], b[8]; volatile unsigned s = 7;
    srand(s); draw(a, 8);
    draw(mid, 8);
    srand(s); draw(b, 8);
    CHECK(same(a, b, 8));
    CHECK(!same(a, mid, 8));
}
int main(void)
{
    tc_begin();
    TC_RUN(default_seed_is_one); TC_RUN(repeat); TC_RUN(different_seeds_differ); TC_RUN(reseed_restarts);
    return tc_end();
}
