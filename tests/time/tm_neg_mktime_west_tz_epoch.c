/* TC
 * id:    tm_neg_mktime_west_tz_epoch
 * area:  time
 * kind:  negative
 * funcs: _mktime64
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs mktime: returns -1 if date is before midnight Jan 1 1970 UTC
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

TC_CASE(pst_valid) { struct tm t; settz("TZ=PST8"); mk(&t, 1970, 1, 1, 0, 0, 0, 0); CHECK_EQ_I(_mktime64(&t), 28800); }
TC_CASE(jst_invalid) { struct tm t; settz("TZ=JST-9"); mk(&t, 1970, 1, 1, 0, 0, 0, 0); CHECK_EQ_I(_mktime64(&t), -1); }
int main(void) { tc_begin(); TC_RUN(pst_valid); TC_RUN(jst_invalid); settz("TZ="); return tc_end(); }
