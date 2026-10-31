/* TC
 * id:    tm_mktime64_isdst_minus1
 * area:  time
 * funcs: _mktime64
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs mktime (negative tm_isdst: compute whether DST is in effect)
 * note:  TZ=PST8PDT
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

TC_CASE(july)
{
    struct tm t = mk(2020, 7, 4, 12, 0, 0, -1); settz("TZ=PST8PDT");
    CHECK_EQ_I(_mktime64(&t), 1593864000LL + 7 * 3600);   /* 12:00 PDT == 19:00Z */
    CHECK_EQ_I(t.tm_isdst, 1); CHECK_EQ_I(t.tm_hour, 12);
}
TC_CASE(january)
{
    struct tm t = mk(2020, 1, 15, 12, 0, 0, -1); settz("TZ=PST8PDT");
    CHECK_EQ_I(_mktime64(&t), 1579118400LL);   /* 12:00 PST == 20:00Z */
    CHECK_EQ_I(t.tm_isdst, 0); CHECK_EQ_I(t.tm_hour, 12);
}
TC_CASE(explicit_std_winter)
{
    struct tm t = mk(2020, 1, 15, 12, 0, 0, 0); settz("TZ=PST8PDT");
    CHECK_EQ_I(_mktime64(&t), 1579118400LL);   /* 12:00 PST == 20:00Z */
}
TC_CASE(explicit_dst_summer)
{
    struct tm t = mk(2020, 7, 4, 12, 0, 0, 1); settz("TZ=PST8PDT");
    CHECK_EQ_I(_mktime64(&t), 1593864000LL + 7 * 3600);   /* 12:00 PDT == 19:00Z */
}
int main(void) { tc_begin(); TC_RUN(july); TC_RUN(january); TC_RUN(explicit_std_winter); TC_RUN(explicit_dst_summer); return tc_end(); }
