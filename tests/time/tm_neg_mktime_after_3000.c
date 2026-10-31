/* TC
 * id:    tm_neg_mktime_after_3000
 * area:  time
 * funcs: _mktime64
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mktime64: returns -1 for dates after 23:59:59 Dec 31 3000 UTC
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

TC_CASE(y3001) { struct tm t = mk(3001, 1, 1, 0, 0, 0, 0); settz("TZ=UTC0"); CHECK_EQ_I(_mktime64(&t), -1); }
TC_CASE(year8000) { struct tm t = mk(9900, 1, 1, 0, 0, 0, 0); settz("TZ=UTC0"); CHECK_EQ_I(t.tm_year, 8000); CHECK_EQ_I(_mktime64(&t), -1); }
TC_CASE(last_ok) { struct tm t = mk(3000, 12, 31, 23, 59, 59, 0); settz("TZ=UTC0"); CHECK_EQ_I(_mktime64(&t), 32535215999LL); }
int main(void) { tc_begin(); TC_RUN(y3001); TC_RUN(year8000); TC_RUN(last_ok); return tc_end(); }
