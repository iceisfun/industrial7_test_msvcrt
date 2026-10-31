/* TC
 * id:    tm_strftime_z_zone_utc
 * area:  time
 * funcs: strftime _tzset _putenv
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _tzset, strftime (%Z time-zone name from TZ / tzname)
 * note:  with TZ=UTC0, %Z yields "UTC" for tm_isdst 0.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

TC_CASE(zone_name)
{
    char buf[64]; struct tm t; char *p = (char *)&t; size_t i, n;
    for (i = 0; i < sizeof t; i++) p[i] = 0;
    t.tm_year = 117; t.tm_mon = 2; t.tm_mday = 5; t.tm_isdst = 0;
    CHECK_EQ_I(_putenv("TZ=UTC0"), 0);
    _tzset();
    n = strftime(buf, sizeof buf, "%Z", &t);
    CHECK_EQ_U(n, 3);
    CHECK_STR(buf, "UTC");
}
int main(void) { tc_begin(); TC_RUN(zone_name); return tc_end(); }
