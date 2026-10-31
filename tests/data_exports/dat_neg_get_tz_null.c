/* TC
 * id:    dat_neg_get_tz_null
 * area:  data_exports
 * kind:  negative
 * funcs: _get_daylight _get_timezone _get_dstbias _tzset
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _get_daylight, _get_timezone, _get_dstbias: EINVAL and invalid parameter handler if the pointer is NULL
 */
#include "tc.h"
#include <errno.h>
#include <time.h>

TC_CASE(daylight_null_einval) { volatile int *p = NULL; int b = tc_iph_count; CHECK_EQ_I(_get_daylight((int *)p), EINVAL); CHECK_EQ_I(tc_iph_count - b, 1); }
TC_CASE(timezone_null_einval) { volatile long *p = NULL; int b = tc_iph_count; CHECK_EQ_I(_get_timezone((long *)p), EINVAL); CHECK_EQ_I(tc_iph_count - b, 1); }
TC_CASE(dstbias_null_einval) { volatile long *p = NULL; int b = tc_iph_count; CHECK_EQ_I(_get_dstbias((long *)p), EINVAL); CHECK_EQ_I(tc_iph_count - b, 1); }
TC_CASE(iph_count_3) { CHECK_EQ_I(tc_iph_count, 3); }
TC_CASE(valid_call_after_failure_returns_zero)
{
    int d = -99; long t = -99, b = -99; int c = tc_iph_count;
    CHECK_EQ_I(_get_daylight(&d), 0); CHECK_EQ_I(_get_timezone(&t), 0); CHECK_EQ_I(_get_dstbias(&b), 0);
    CHECK(d != -99); CHECK(t != -99); CHECK(b != -99);
    CHECK_EQ_I(tc_iph_count, c);
}

int main(void)
{
    tc_begin(); tc_iph_install(); _tzset();
    TC_RUN(daylight_null_einval); TC_RUN(timezone_null_einval); TC_RUN(dstbias_null_einval); TC_RUN(iph_count_3);
    TC_RUN(valid_call_after_failure_returns_zero);
    return tc_end();
}
