/* TC
 * id:    tm_asctime_year_edges
 * area:  time
 * funcs: asctime
 * procs: 
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.27.3.1 (%d of tm_year+1900)
 * note:  year formatting
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s, int wd)
{
    volatile int *p; t->tm_year = y; t->tm_mon = mo; t->tm_mday = d; t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_wday = wd; t->tm_yday = 0; t->tm_isdst = 0; (void)p;
}
static struct tm t;
TC_CASE(y1900) { mk(&t, 0, 0, 15, 0, 0, 0, 1); CHECK_STR(asctime(&t), "Mon Jan 15 00:00:00 1900\n"); }
TC_CASE(y2000) { mk(&t, 100, 0, 15, 0, 0, 0, 6); CHECK_STR(asctime(&t), "Sat Jan 15 00:00:00 2000\n"); }
TC_CASE(y3000) { mk(&t, 1100, 0, 15, 0, 0, 0, 3); CHECK_STR(asctime(&t), "Wed Jan 15 00:00:00 3000\n"); }
int main(void) { tc_begin(); TC_RUN(y1900); TC_RUN(y2000); TC_RUN(y3000); return tc_end(); }
