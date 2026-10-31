/* TC
 * id:    tm_mktime64_normalize
 * area:  time
 * funcs: _mktime64
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.27.2.3 (mktime ranges are not restricted)
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

#define EXPECT(t, y, mo, d, h, mi, s) do { CHECK_EQ_I((t).tm_year, (y) - 1900); CHECK_EQ_I((t).tm_mon, (mo) - 1); \
    CHECK_EQ_I((t).tm_mday, d); CHECK_EQ_I((t).tm_hour, h); CHECK_EQ_I((t).tm_min, mi); CHECK_EQ_I((t).tm_sec, s); } while (0)

TC_CASE(mon12) { struct tm t = mk(2020, 1, 15, 10, 0, 0, 0); settz("TZ=UTC0"); t.tm_mon = 12; CHECK(_mktime64(&t) != -1); EXPECT(t, 2021, 1, 15, 10, 0, 0); }
TC_CASE(mday32) { struct tm t = mk(2020, 1, 32, 0, 0, 0, 0); settz("TZ=UTC0"); CHECK(_mktime64(&t) != -1); EXPECT(t, 2020, 2, 1, 0, 0, 0); }
TC_CASE(hour25) { struct tm t = mk(2020, 3, 10, 25, 0, 0, 0); settz("TZ=UTC0"); CHECK(_mktime64(&t) != -1); EXPECT(t, 2020, 3, 11, 1, 0, 0); }
TC_CASE(min_neg1) { struct tm t = mk(2020, 3, 10, 5, -1, 0, 0); settz("TZ=UTC0"); CHECK(_mktime64(&t) != -1); EXPECT(t, 2020, 3, 10, 4, 59, 0); }
TC_CASE(sec3600) { struct tm t = mk(2020, 3, 10, 5, 0, 3600, 0); settz("TZ=UTC0"); CHECK(_mktime64(&t) != -1); EXPECT(t, 2020, 3, 10, 6, 0, 0); }
TC_CASE(mday0)
{
    struct tm t = mk(2020, 3, 0, 0, 0, 0, 0); settz("TZ=UTC0");
    CHECK(_mktime64(&t) != -1);
    EXPECT(t, 2020, 2, 29, 0, 0, 0);        /* 2020 is a leap year */
    CHECK_EQ_I(t.tm_yday, 59); CHECK_EQ_I(t.tm_wday, 6);   /* Saturday */
}
int main(void) { tc_begin(); TC_RUN(mon12); TC_RUN(mday32); TC_RUN(hour25); TC_RUN(min_neg1); TC_RUN(sec3600); TC_RUN(mday0); return tc_end(); }
