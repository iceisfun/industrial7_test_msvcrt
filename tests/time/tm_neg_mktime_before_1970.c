/* TC
 * id:    tm_neg_mktime_before_1970
 * area:  time
 * funcs: _mktime64
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mktime64: returns -1 for dates before Jan 1 1970 UTC
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

TC_CASE(minus_one_second) { struct tm t = mk(1969, 12, 31, 23, 59, 59, 0); settz("TZ=UTC0"); CHECK_EQ_I(_mktime64(&t), -1); }
TC_CASE(year0) { struct tm t; memset(&t, 0, sizeof t); settz("TZ=UTC0"); t.tm_mday = 1; CHECK_EQ_I(_mktime64(&t), -1); }
TC_CASE(year69) { struct tm t = mk(1969, 1, 1, 0, 0, 0, 0); settz("TZ=UTC0"); CHECK_EQ_I(t.tm_year, 69); CHECK_EQ_I(_mktime64(&t), -1); }
TC_CASE(boundary_ok) { struct tm t = mk(1970, 1, 1, 0, 0, 0, 0); settz("TZ=UTC0"); CHECK_EQ_I(_mktime64(&t), 0); }
int main(void) { tc_begin(); TC_RUN(minus_one_second); TC_RUN(year0); TC_RUN(year69); TC_RUN(boundary_ok); return tc_end(); }
