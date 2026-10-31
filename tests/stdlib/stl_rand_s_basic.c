/* TC
 * id:    stl_rand_s_basic
 * area:  stdlib
 * funcs: rand_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs rand_s (cryptographically secure, independent of srand, returns 0 on success, 0..UINT_MAX)
 */
#define _CRT_RAND_S
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(success_returns_zero)
{
    unsigned int v = 0xDEADBEEFu; errno_t r;
    errno = 0;
    r = rand_s(&v);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(errno, 0);
}
TC_CASE(distinct_values)
{
    unsigned int first = 0, v; int i, diff = 0, bad = 0;
    for (i = 0; i < 1000; i++) {
        if (rand_s(&v) != 0) { bad++; continue; }
        if (i == 0) first = v; else if (v != first) diff++;
    }
    CHECK_EQ_I(bad, 0);
    CHECK(diff > 0);
}
TC_CASE(independent_of_srand)
{
    unsigned int a[8], b[8]; int i, same = 1;
    srand(1); for (i = 0; i < 8; i++) rand_s(&a[i]);
    srand(1); for (i = 0; i < 8; i++) rand_s(&b[i]);
    for (i = 0; i < 8; i++) if (a[i] != b[i]) same = 0;
    CHECK(!same);   /* chance of 8 identical 32-bit values is 2^-256 */
}
TC_CASE(high_bit_seen)
{
    int i, seen = 0; unsigned int v;
    for (i = 0; i < 200; i++) if (rand_s(&v) == 0 && (v & 0x80000000u)) seen = 1;
    CHECK(seen);
}
int main(void) { tc_begin(); TC_RUN(success_returns_zero); TC_RUN(distinct_values); TC_RUN(independent_of_srand); TC_RUN(high_bit_seen); return tc_end(); }
