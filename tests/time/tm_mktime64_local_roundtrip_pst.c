/* TC
 * id:    tm_mktime64_local_roundtrip_pst
 * area:  time
 * funcs: _mktime64 _localtime64
 * crt:   all
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.27.3.4, 7.27.2.3
 * note:  TZ=PST8PDT; timestamps are mid-January and early July, away from DST transitions
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

TC_CASE(winter)
{
    volatile __time64_t t = 1579118400LL; struct tm *l; struct tm c; settz("TZ=PST8PDT");
    l = _localtime64((const __time64_t *)&t); CHECK(l != NULL); if (!l) return;
    c = *l;
    CHECK_EQ_I(c.tm_isdst, 0); CHECK_EQ_I(c.tm_hour, 12);
    CHECK_EQ_I(_mktime64(&c), 1579118400LL); CHECK_EQ_I(c.tm_isdst, 0);
}
TC_CASE(summer)
{
    volatile __time64_t t = 1593864000LL; struct tm *l; struct tm c; settz("TZ=PST8PDT");
    l = _localtime64((const __time64_t *)&t); CHECK(l != NULL); if (!l) return;
    c = *l;
    CHECK_EQ_I(c.tm_isdst, 1); CHECK_EQ_I(c.tm_hour, 5);
    CHECK_EQ_I(_mktime64(&c), 1593864000LL); CHECK_EQ_I(c.tm_isdst, 1);
}
int main(void) { tc_begin(); TC_RUN(winter); TC_RUN(summer); return tc_end(); }
