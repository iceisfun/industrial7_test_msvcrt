/* TC
 * id:    tm_neg_mktime_null
 * area:  time
 * kind:  negative
 * funcs: _mktime64
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs mktime, _mktime32, _mktime64 (NULL tm -> EINVAL, returns -1)
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

TC_CASE(null_tm)
{
    __time64_t r; struct tm *volatile p = NULL;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _mktime64(p);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(null_tm); return tc_end(); }
