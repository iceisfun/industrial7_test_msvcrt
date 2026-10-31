/* TC
 * id:    tm_mkgmtime64_normalize
 * area:  time
 * kind:  positive
 * funcs: _mkgmtime64
 * crt:   >=80
 * tier:  p1
 * conf:  inferred
 * ref:   MS docs mktime/_mkgmtime64 (out-of-range fields are normalized)
 * note:  only return values asserted; struct write-back checked in tm_mkgmtime64_probe_fields
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static void settz(const char *s) { _putenv(s); _tzset(); }
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s, int dst)
{
    volatile int v = y; t->tm_year = v - 1900; t->tm_mon = mo - 1; t->tm_mday = d;
    t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_isdst = dst; t->tm_wday = 0; t->tm_yday = 0;
}

TC_CASE(month_12)
{
    struct tm t; mk(&t, 1999, 1, 1, 0, 0, 0, 1); t.tm_mon = 12;
    CHECK_EQ_I(_mkgmtime64(&t), 946684800);
}
TC_CASE(hour_48)
{
    struct tm t; mk(&t, 2000, 1, 1, 48, 0, 0, 0);
    CHECK_EQ_I(_mkgmtime64(&t), 946857600);
}
TC_CASE(mday_0)
{
    struct tm t; mk(&t, 2000, 3, 0, 0, 0, 0, 0);
    CHECK_EQ_I(_mkgmtime64(&t), 951782400);
}
TC_CASE(minute_neg)
{
    struct tm t; mk(&t, 2000, 1, 1, 0, 0, 0, 0); t.tm_min = -1;
    CHECK_EQ_I(_mkgmtime64(&t), 946684740);
}
int main(void) { tc_begin(); TC_RUN(month_12); TC_RUN(hour_48); TC_RUN(mday_0); TC_RUN(minute_neg); return tc_end(); }
