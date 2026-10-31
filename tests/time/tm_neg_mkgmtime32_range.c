/* TC
 * id:    tm_neg_mkgmtime32_range
 * area:  time
 * kind:  negative
 * funcs: _mkgmtime32
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _mkgmtime32: range 1970-01-01 to 2038-01-18 UTC; NULL -> EINVAL (VS2005+)
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

TC_CASE(past_max) { struct tm t; mk(&t, 2038, 1, 19, 3, 14, 8, 0); CHECK_EQ_I(_mkgmtime32(&t), -1); }
TC_CASE(pre_1970) { struct tm t; mk(&t, 1969, 12, 31, 23, 59, 59, 0); CHECK_EQ_I(_mkgmtime32(&t), -1); }
TC_CASE(null_tm)
{
    __time32_t r; struct tm *volatile p = NULL;
    if (!TC_AT_LEAST(90) || !tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _mkgmtime32(p);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(past_max); TC_RUN(pre_1970); TC_RUN(null_tm); return tc_end(); }
