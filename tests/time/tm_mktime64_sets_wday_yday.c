/* TC
 * id:    tm_mktime64_sets_wday_yday
 * area:  time
 * funcs: _mktime64
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.27.2.3 (tm_wday and tm_yday are set, input values ignored)
 * note:  TZ=UTC0
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

static void settz(const char *s) { _putenv(s); _tzset(); }
static struct tm mk(int y, int mo, int d, int h, int mi, int s, int dst)
{
    struct tm t; memset(&t, 0, sizeof t);
    t.tm_year = y - 1900; t.tm_mon = mo - 1; t.tm_mday = d; t.tm_hour = h; t.tm_min = mi; t.tm_sec = s; t.tm_isdst = dst;
    return t;
}

TC_CASE(garbage_wday) { struct tm t = mk(2000, 2, 29, 12, 0, 0, 0); settz("TZ=UTC0"); t.tm_wday = 99; t.tm_yday = 999;
    CHECK(_mktime64(&t) != -1); CHECK_EQ_I(t.tm_wday, 2); CHECK_EQ_I(t.tm_yday, 59); }
TC_CASE(leap_day) { struct tm t = mk(2000, 2, 29, 0, 0, 0, 0); settz("TZ=UTC0"); CHECK(_mktime64(&t) != -1); CHECK_EQ_I(t.tm_yday, 59); CHECK_EQ_I(t.tm_wday, 2); }
TC_CASE(y2100) { struct tm t = mk(2100, 3, 1, 0, 0, 0, 0); settz("TZ=UTC0"); t.tm_wday = -5; t.tm_yday = -5;
    CHECK(_mktime64(&t) != -1); CHECK_EQ_I(t.tm_yday, 59); CHECK_EQ_I(t.tm_wday, 1); }
TC_CASE(epoch) { struct tm t = mk(1970, 1, 1, 0, 0, 0, 0); settz("TZ=UTC0"); t.tm_wday = 6; t.tm_yday = 77;
    CHECK_EQ_I(_mktime64(&t), 0); CHECK_EQ_I(t.tm_wday, 4); CHECK_EQ_I(t.tm_yday, 0); }
int main(void) { tc_begin(); TC_RUN(garbage_wday); TC_RUN(leap_day); TC_RUN(y2100); TC_RUN(epoch); return tc_end(); }
