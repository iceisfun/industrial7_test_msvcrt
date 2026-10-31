/* TC
 * id:    tm_mktime64_probe_extreme_fields
 * area:  time
 * funcs: _mktime64 _mkgmtime64 _mkgmtime32
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs mktime, _mkgmtime64 (-1 when the calendar time cannot be represented)
 * note:  PROBE: normalising INT_MAX / INT_MIN in tm_min, tm_hour, tm_mday, tm_mon (all land far beyond year 3000 or before
 *        1970 if computed in 64-bit arithmetic) yields -1 rather than a wrapped in-range value. Whether every CRT generation
 *        does the intermediate arithmetic in 64 bits is the thing being probed. TZ=UTC0.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <limits.h>
static volatile int vi;
static struct tm t;
static void good(void) { t.tm_year = 100; t.tm_mon = 0; t.tm_mday = 1; t.tm_hour = 12; t.tm_min = 0; t.tm_sec = 0; t.tm_wday = 0; t.tm_yday = 0; t.tm_isdst = 0; }
static void setup(void) { _putenv("TZ=UTC0"); _tzset(); good(); }
TC_CASE(mktime_max)
{
    setup(); vi = INT_MAX; t.tm_min = vi; CHECK_EQ_I(_mktime64(&t), -1);
    setup(); vi = INT_MAX; t.tm_hour = vi; CHECK_EQ_I(_mktime64(&t), -1);
    setup(); vi = INT_MAX; t.tm_mday = vi; CHECK_EQ_I(_mktime64(&t), -1);
    setup(); vi = INT_MAX; t.tm_mon = vi; CHECK_EQ_I(_mktime64(&t), -1);
}
TC_CASE(mktime_min)
{
    setup(); vi = INT_MIN; t.tm_mday = vi; CHECK_EQ_I(_mktime64(&t), -1);
    setup(); vi = INT_MIN; t.tm_mon = vi; CHECK_EQ_I(_mktime64(&t), -1);
}
TC_CASE(mkgm_max)
{
    good(); vi = INT_MAX; t.tm_min = vi; CHECK_EQ_I(_mkgmtime64(&t), -1);
    good(); vi = INT_MAX; t.tm_hour = vi; CHECK_EQ_I(_mkgmtime64(&t), -1);
    good(); vi = INT_MAX; t.tm_mday = vi; CHECK_EQ_I(_mkgmtime64(&t), -1);
    good(); vi = INT_MAX; t.tm_mon = vi; CHECK_EQ_I(_mkgmtime64(&t), -1);
}
TC_CASE(mkgm_min)
{
    good(); vi = INT_MIN; t.tm_mday = vi; CHECK_EQ_I(_mkgmtime64(&t), -1);
    good(); vi = INT_MIN; t.tm_mon = vi; CHECK_EQ_I(_mkgmtime64(&t), -1);
}
TC_CASE(mkgm32_mday_max) { good(); vi = INT_MAX; t.tm_mday = vi; CHECK_EQ_I(_mkgmtime32(&t), -1); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(mktime_max); TC_RUN(mktime_min); TC_RUN(mkgm_max); TC_RUN(mkgm_min); TC_RUN(mkgm32_mday_max);
    return tc_end();
}
