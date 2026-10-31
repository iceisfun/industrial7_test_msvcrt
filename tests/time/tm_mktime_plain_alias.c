/* TC
 * id:    tm_mktime_plain_alias
 * area:  time
 * funcs: _mktime64
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs mktime
 * note:  In the 64-bit build the header maps mktime to _mktime64 (inline), (x64 mingw), so the import is _mktime64. TZ=UTC0
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

TC_CASE(known) { struct tm t = mk(2000, 2, 29, 0, 0, 0, 0); settz("TZ=UTC0"); CHECK_EQ_I(mktime(&t), 951782400LL); }
TC_CASE(mon13) { struct tm t = mk(2020, 1, 15, 0, 0, 0, 0); settz("TZ=UTC0"); t.tm_mon = 13;
    CHECK(mktime(&t) != (time_t)-1); CHECK_EQ_I(t.tm_year, 121); CHECK_EQ_I(t.tm_mon, 1); CHECK_EQ_I(t.tm_mday, 15); }
TC_CASE(wday_yday) { struct tm t = mk(2000, 2, 29, 0, 0, 0, 0); settz("TZ=UTC0"); t.tm_wday = 9; t.tm_yday = 9;
    CHECK(mktime(&t) != (time_t)-1); CHECK_EQ_I(t.tm_wday, 2); CHECK_EQ_I(t.tm_yday, 59); }
int main(void) { tc_begin(); TC_RUN(known); TC_RUN(mon13); TC_RUN(wday_yday); return tc_end(); }
