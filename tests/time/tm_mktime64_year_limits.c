/* TC
 * id:    tm_mktime64_year_limits
 * area:  time
 * funcs: _mktime64
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mktime64 (valid range: Jan 1 1970 to 23:59:59 Dec 31 3000 UTC)
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

TC_CASE(epoch) { struct tm t = mk(1970, 1, 1, 0, 0, 0, 0); settz("TZ=UTC0"); CHECK_EQ_I(t.tm_year, 70); CHECK_EQ_I(_mktime64(&t), 0); }
TC_CASE(max) { struct tm t = mk(3000, 12, 31, 23, 59, 59, 0); settz("TZ=UTC0"); CHECK_EQ_I(_mktime64(&t), 32535215999LL); }
TC_CASE(year1100) { struct tm t; memset(&t, 0, sizeof t); settz("TZ=UTC0");
    t.tm_year = 1100; t.tm_mon = 11; t.tm_mday = 31;
    CHECK_EQ_I(_mktime64(&t), 32535129600LL); CHECK_EQ_I(t.tm_yday, 364); }
int main(void) { tc_begin(); TC_RUN(epoch); TC_RUN(max); TC_RUN(year1100); return tc_end(); }
