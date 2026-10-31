/* TC
 * id:    tm_asctime_names
 * area:  time
 * funcs: asctime
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.27.3.1
 * note:  weekday/month names
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s, int wd)
{
    volatile int *p; t->tm_year = y; t->tm_mon = mo; t->tm_mday = d; t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_wday = wd; t->tm_yday = 0; t->tm_isdst = 0; (void)p;
}
static struct tm t;
TC_CASE(weekdays)
{
    static const char *w[7] = { "Sun Jan 15 00:00:00 2017\n", "Mon Jan 15 00:00:00 2017\n", "Tue Jan 15 00:00:00 2017\n", "Wed Jan 15 00:00:00 2017\n", "Thu Jan 15 00:00:00 2017\n", "Fri Jan 15 00:00:00 2017\n", "Sat Jan 15 00:00:00 2017\n" };
    int i;
    for (i = 0; i < 7; i++) { mk(&t, 117, 0, 15, 0, 0, 0, i); CHECK_STR(asctime(&t), w[i]); }
}
TC_CASE(months)
{
    static const char *m[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    int i;
    for (i = 0; i < 12; i++) {
        char want[32]; const char *p = "Sun "; int k = 0, j;
        for (j = 0; p[j]; j++) want[k++] = p[j];
        for (j = 0; j < 3; j++) want[k++] = m[i][j];
        { const char *r = " 15 00:00:00 2017\n"; for (j = 0; r[j]; j++) want[k++] = r[j]; }
        want[k] = 0;
        mk(&t, 117, i, 15, 0, 0, 0, 0);
        CHECK_STR(asctime(&t), want);
    }
}
int main(void) { tc_begin(); TC_RUN(weekdays); TC_RUN(months); return tc_end(); }
