/* TC
 * id:    tm_neg_mkgmtime64_range
 * area:  time
 * kind:  negative
 * funcs: _mkgmtime64
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mkgmtime64: range 1970-01-01 to 3000-12-31 23:59:59 UTC
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

TC_CASE(pre_epoch) { struct tm t; mk(&t, 1969, 12, 31, 23, 59, 59, 0); CHECK_EQ_I(_mkgmtime64(&t), -1); }
TC_CASE(past_3000) { struct tm t; mk(&t, 3001, 1, 1, 0, 0, 0, 0); CHECK_EQ_I(_mkgmtime64(&t), -1); }
TC_CASE(year_1900) { struct tm t; mk(&t, 1900, 1, 1, 0, 0, 0, 0); CHECK_EQ_I(_mkgmtime64(&t), -1); }
TC_CASE(boundary) { struct tm t; mk(&t, 3000, 12, 31, 23, 59, 59, 0); CHECK_EQ_I(_mkgmtime64(&t), 32535215999LL); }
int main(void) { tc_begin(); TC_RUN(pre_epoch); TC_RUN(past_3000); TC_RUN(year_1900); TC_RUN(boundary); return tc_end(); }
