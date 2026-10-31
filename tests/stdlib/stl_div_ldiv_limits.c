/* TC
 * id:    stl_div_ldiv_limits
 * area:  stdlib
 * funcs: div ldiv
 * crt:   all
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.22.6.2 (quotient truncated toward zero, quot*d+rem == n); MS docs div, ldiv
 * note:  Boundary operands whose quotients are representable (positive test: nothing here may trap). The traps are in stl_neg_div_trap_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

static volatile int vi_min = INT_MIN, vi_max = INT_MAX, v_one = 1, v_m1 = -1;
static volatile long vl_min = LONG_MIN, vl_max = LONG_MAX;

TC_CASE(div_min_by_1)   { div_t r = div(vi_min, v_one); CHECK_EQ_I(r.quot, INT_MIN); CHECK_EQ_I(r.rem, 0); }
TC_CASE(div_max_by_m1)  { div_t r = div(vi_max, v_m1);  CHECK_EQ_I(r.quot, -INT_MAX); CHECK_EQ_I(r.rem, 0); }
TC_CASE(div_min_by_max) { div_t r = div(vi_min, vi_max); CHECK_EQ_I(r.quot, -1); CHECK_EQ_I(r.rem, -1); }
TC_CASE(div_trunc_toward_zero)
{
    volatile int a = -7, b = 2, c = 7, d = -2; div_t r;
    r = div(a, b); CHECK_EQ_I(r.quot, -3); CHECK_EQ_I(r.rem, -1);
    r = div(c, d); CHECK_EQ_I(r.quot, -3); CHECK_EQ_I(r.rem, 1);
    r = div(a, d); CHECK_EQ_I(r.quot, 3);  CHECK_EQ_I(r.rem, -1);
}
TC_CASE(ldiv_min_max)
{
    ldiv_t r = ldiv(vl_min, vl_max); CHECK_EQ_I(r.quot, -1); CHECK_EQ_I(r.rem, -1);
    r = ldiv(vl_max, -1L); CHECK_EQ_I(r.quot, -LONG_MAX); CHECK_EQ_I(r.rem, 0);
    r = ldiv(vl_min, 1L); CHECK_EQ_I(r.quot, LONG_MIN); CHECK_EQ_I(r.rem, 0);
}
int main(void)
{
    tc_begin(); TC_RUN(div_min_by_1); TC_RUN(div_max_by_m1); TC_RUN(div_min_by_max); TC_RUN(div_trunc_toward_zero);
    TC_RUN(ldiv_min_max); return tc_end();
}
