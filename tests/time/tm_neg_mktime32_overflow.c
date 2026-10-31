/* TC
 * id:    tm_neg_mktime32_overflow
 * area:  time
 * kind:  negative
 * funcs: _mktime32
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs mktime: _mktime32 range ends 03:14:07 Jan 19 2038 UTC
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

TC_CASE(last_valid) { struct tm t; settz("TZ=UTC0"); mk(&t, 2038, 1, 19, 3, 14, 7, 0); CHECK_EQ_I(_mktime32(&t), 2147483647); }
TC_CASE(one_past) { struct tm t; settz("TZ=UTC0"); mk(&t, 2038, 1, 19, 3, 14, 8, 0); CHECK_EQ_I(_mktime32(&t), -1); }
TC_CASE(year_2100) { struct tm t; settz("TZ=UTC0"); mk(&t, 2100, 1, 1, 0, 0, 0, 0); CHECK_EQ_I(_mktime32(&t), -1); }
TC_CASE(before_1970) { struct tm t; settz("TZ=UTC0"); mk(&t, 1969, 12, 31, 23, 59, 59, 0); CHECK_EQ_I(_mktime32(&t), -1); }
int main(void) { tc_begin(); TC_RUN(last_valid); TC_RUN(one_past); TC_RUN(year_2100); TC_RUN(before_1970); return tc_end(); }
