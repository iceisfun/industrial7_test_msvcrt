/* TC
 * id:    tm_tzset_no_dst_daylight
 * area:  time
 * funcs:
 * procs: _get_daylight _get_timezone
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _tzset, _get_daylight, _get_timezone
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

TC_DECL_PROC(int, _get_daylight, (int *));
TC_DECL_PROC(int, _get_timezone, (long *));

TC_CASE(est5)
{
    int d = 77; long z = 77; int r1, r2;
    if (!tc_p__get_daylight || !tc_p__get_timezone) TC_SKIP("getters missing");
    _putenv("TZ=EST5"); _tzset();
    r1 = tc_p__get_daylight(&d); r2 = tc_p__get_timezone(&z);
    CHECK_EQ_I(r1, 0); CHECK_EQ_I(r2, 0);
    CHECK_EQ_I(d, 0);
    CHECK_EQ_I(z, 18000);
}
TC_CASE(pst8pdt)
{
    int d = 77; long z = 77;
    if (!tc_p__get_daylight || !tc_p__get_timezone) TC_SKIP("getters missing");
    _putenv("TZ=PST8PDT"); _tzset();
    CHECK_EQ_I(tc_p__get_daylight(&d), 0); CHECK_EQ_I(tc_p__get_timezone(&z), 0);
    CHECK_EQ_I(d, 1);
    CHECK_EQ_I(z, 28800);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_get_daylight); TC_LOAD_PROC(_get_timezone);
    TC_RUN(est5); TC_RUN(pst8pdt);
    _putenv("TZ="); _tzset();
    return tc_end();
}
