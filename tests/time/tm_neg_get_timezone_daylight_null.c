/* TC
 * id:    tm_neg_get_timezone_daylight_null
 * area:  time
 * funcs:
 * procs: _get_timezone _get_daylight
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _get_timezone, _get_daylight (return value: EINVAL if the pointer is NULL)
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

TC_DECL_PROC(int, _get_daylight, (int *));
TC_DECL_PROC(int, _get_timezone, (long *));

TC_CASE(timezone_null)
{
    int r;
    if (!tc_p__get_timezone) TC_SKIP("missing");
    tc_iph_count = 0; errno = 0;
    r = tc_p__get_timezone(NULL);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(daylight_null)
{
    int r;
    if (!tc_p__get_daylight) TC_SKIP("missing");
    tc_iph_count = 0; errno = 0;
    r = tc_p__get_daylight(NULL);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_LOAD_PROC(_get_timezone); TC_LOAD_PROC(_get_daylight);
    TC_RUN(timezone_null); TC_RUN(daylight_null);
    return tc_end();
}
