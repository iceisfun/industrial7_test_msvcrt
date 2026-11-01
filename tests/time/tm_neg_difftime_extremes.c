/* TC
 * id:    tm_neg_difftime_extremes
 * area:  time
 * kind:  negative
 * funcs: _difftime64
 * crt:   >=90
 * tier:  p0
 * conf:  observed
 * ref:   MS docs difftime, _difftime32, _difftime64 (returns time1 - time0 as double; no error return)
 * note:  Extreme arguments must not crash or invoke the invalid parameter handler; results are exact integer
 *        differences converted to double.
 *        ucrtbase _difftime64(LLONG_MIN, 0) returns +0.0 on Win10 22H2 (any negative operand gives 0.0 with errno EINVAL and no handler call), not -9.22e18.
 */
#include "tc.h"
#include <time.h>
#include <limits.h>
#include <errno.h>

static volatile __time64_t v_max = LLONG_MAX, v_min = LLONG_MIN, v_zero = 0, v_five = 5;

TC_CASE(zero_minus_max) { double r = _difftime64(v_zero, v_max); CHECK_BITS_D(r, -(double)LLONG_MAX); }
/* observed on Win10 ucrtbase: _difftime64 returns +0.0 whenever an argument is negative (deviates from "time1 - time0", C11 7.27.2.2) */
TC_CASE(min_minus_zero) { double r; errno = 0; r = _difftime64(v_min, v_zero);
#if TC_CRT_VER >= 140
    CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, EINVAL);
#else
    CHECK_BITS_D(r, (double)LLONG_MIN);
#endif
}
TC_CASE(same) { double r = _difftime64(v_five, v_five); CHECK_BITS_D(r, 0.0); }
/* min - max is deliberately not computed: the difference overflows int64 */
TC_CASE(iph_untouched) { tc_iph_count = 0; (void)_difftime64(v_zero, v_max); (void)_difftime64(v_min, v_zero); CHECK_EQ_I(tc_iph_count, 0); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(zero_minus_max); TC_RUN(min_minus_zero); TC_RUN(same); TC_RUN(iph_untouched);
    return tc_end();
}
