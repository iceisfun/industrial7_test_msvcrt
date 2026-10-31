/* TC
 * id:    tm_tzset_utc
 * area:  time
 * funcs: _tzset _get_timezone _get_daylight _get_tzname
 * crt:   >=90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _tzset, _get_timezone, _get_daylight, _get_tzname
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

TC_CASE(utc)
{
    long tz = 99; int dl = 99; char buf[16]; size_t n = 0; buf[0] = 0;
    _putenv("TZ=UTC0"); _tzset();
    CHECK_EQ_I(_get_timezone(&tz), 0); CHECK_EQ_I(tz, 0);
    CHECK_EQ_I(_get_daylight(&dl), 0); CHECK_EQ_I(dl, 0);
    CHECK_EQ_I(_get_tzname(&n, buf, sizeof buf, 0), 0); CHECK_EQ_U(n, 4); CHECK_STR(buf, "UTC");
}
TC_CASE(reflects_change)
{
    long tz = 99;
    _putenv("TZ=UTC0"); _tzset(); _get_timezone(&tz); CHECK_EQ_I(tz, 0);
    _putenv("TZ=EST5"); _tzset(); _get_timezone(&tz); CHECK_EQ_I(tz, 18000);
    _putenv("TZ=UTC0"); _tzset(); _get_timezone(&tz); CHECK_EQ_I(tz, 0);
}
int main(void) { tc_begin(); TC_RUN(utc); TC_RUN(reflects_change); return tc_end(); }
