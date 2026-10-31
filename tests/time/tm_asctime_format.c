/* TC
 * id:    tm_asctime_format
 * area:  time
 * funcs: asctime
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.27.3.1; MS docs asctime
 * note: day 15 used so output is identical on every CRT; see tm_asctime_day_pad*
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s, int wd)
{
    volatile int *p; t->tm_year = y; t->tm_mon = mo; t->tm_mday = d; t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_wday = wd; t->tm_yday = 0; t->tm_isdst = 0; (void)p;
}
static struct tm t;
TC_CASE(sunday) { mk(&t, 117, 0, 15, 0, 0, 0, 0); CHECK_STR(asctime(&t), "Sun Jan 15 00:00:00 2017\n"); }
TC_CASE(two_digit_day) { mk(&t, 117, 9, 23, 14, 5, 9, 1); CHECK_STR(asctime(&t), "Mon Oct 23 14:05:09 2017\n"); }
TC_CASE(zero_pad) { mk(&t, 120, 1, 29, 1, 2, 3, 6); CHECK_STR(asctime(&t), "Sat Feb 29 01:02:03 2020\n"); }
TC_CASE(length) { char *s; mk(&t, 117, 0, 15, 0, 0, 0, 0); s = asctime(&t); CHECK(s != NULL); if (s) { size_t n = 0; while (s[n]) n++; CHECK_EQ_U(n, 25); CHECK_EQ_I(s[24], '\n'); } }
int main(void) { tc_begin(); TC_RUN(sunday); TC_RUN(two_digit_day); TC_RUN(zero_pad); TC_RUN(length); return tc_end(); }
