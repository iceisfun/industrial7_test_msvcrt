/* TC
 * id:    tm_neg_asctime_invalid_fields
 * area:  time
 * funcs: asctime
 * procs: 
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs asctime_s error table (ranges)
 * note:  out-of-range tm fields; each must give NULL/EINVAL/iph
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s, int wd)
{
    volatile int *p; t->tm_year = y; t->tm_mon = mo; t->tm_mday = d; t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_wday = wd; t->tm_yday = 0; t->tm_isdst = 0; (void)p;
}
static struct tm t;
static void bad(void)
{
    char *r; errno = 0; tc_iph_count = 0;
    r = asctime(&t);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
#define GOOD mk(&t, 117, 9, 23, 14, 5, 9, 1)
TC_CASE(mon) { GOOD; t.tm_mon = 12; bad(); GOOD; t.tm_mon = -1; bad(); }
TC_CASE(mday) { GOOD; t.tm_mday = 0; bad(); GOOD; t.tm_mday = 32; bad(); }
TC_CASE(hour) { GOOD; t.tm_hour = 24; bad(); GOOD; t.tm_hour = -1; bad(); }
TC_CASE(minsec) { GOOD; t.tm_min = 60; bad(); GOOD; t.tm_min = -1; bad(); GOOD; t.tm_sec = -1; bad(); }
TC_CASE(wday) { GOOD; t.tm_wday = 7; bad(); GOOD; t.tm_wday = -1; bad(); }
TC_CASE(year) { GOOD; t.tm_year = -1; bad(); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(mon); TC_RUN(mday); TC_RUN(hour); TC_RUN(minsec); TC_RUN(wday); TC_RUN(year); return tc_end(); }
