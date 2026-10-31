/* TC
 * id:    tm_neg_asctime_null
 * area:  time
 * funcs: asctime
 * procs: 
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs asctime (NULL -> invalid parameter, EINVAL)
 * note:  asctime(NULL)
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s, int wd)
{
    volatile int *p; t->tm_year = y; t->tm_mon = mo; t->tm_mday = d; t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_wday = wd; t->tm_yday = 0; t->tm_isdst = 0; (void)p;
}
TC_CASE(null)
{
    char *r;
    errno = 0; tc_iph_count = 0;
    r = asctime((const struct tm *)(volatile void *)0);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(null); return tc_end(); }
