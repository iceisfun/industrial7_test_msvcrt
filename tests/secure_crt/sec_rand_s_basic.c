/* TC
 * id:    sec_rand_s_basic
 * area:  secure_crt
 * funcs:
 * procs: rand_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs rand_s: writes a random unsigned int in [0, UINT_MAX]; returns zero on success
 * note:  Probabilistic: 64-call checks have a 2^-64 false-fail chance.
 */
#include "tc.h"

typedef int (__cdecl *rand_s_t)(unsigned int *);
static rand_s_t p_rand_s;

TC_CASE(returns_0) { unsigned int v = 0; CHECK_EQ_I(p_rand_s(&v), 0); }
TC_CASE(not_all_equal_over_64_calls)
{
    unsigned int first = 0, v = 0; int i, diff = 0;
    CHECK_EQ_I(p_rand_s(&first), 0);
    for (i = 1; i < 64; i++) { CHECK_EQ_I(p_rand_s(&v), 0); if (v != first) diff = 1; }
    CHECK(diff);
}
TC_CASE(uses_full_32_bit_range_has_high_bit_set_sometimes)
{
    unsigned int v = 0, orv = 0, andv = 0xffffffffu; int i;
    for (i = 0; i < 64; i++) { CHECK_EQ_I(p_rand_s(&v), 0); orv |= v; andv &= v; }
    CHECK(orv & 0x80000000u);
    CHECK(!(andv & 0x80000000u));
}
int main(void)
{
    tc_begin();
    p_rand_s = (rand_s_t)tc_crt_proc("rand_s");
    CHECK(p_rand_s); if (!p_rand_s) return tc_end();
    TC_RUN(returns_0); TC_RUN(not_all_equal_over_64_calls); TC_RUN(uses_full_32_bit_range_has_high_bit_set_sometimes);
    return tc_end();
}
