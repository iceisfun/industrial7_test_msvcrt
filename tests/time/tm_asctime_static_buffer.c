/* TC
 * id:    tm_asctime_static_buffer
 * area:  time
 * funcs: asctime
 * procs: 
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  documented
 * ref:   MS docs asctime (static buffer)
 * note:  static result buffer
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s, int wd)
{
    volatile int *p; t->tm_year = y; t->tm_mon = mo; t->tm_mday = d; t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_wday = wd; t->tm_yday = 0; t->tm_isdst = 0; (void)p;
}
static struct tm t1, t2;
TC_CASE(same_pointer) { char *a, *b; mk(&t1, 117, 0, 15, 0, 0, 0, 0); mk(&t2, 120, 1, 29, 1, 2, 3, 6); a = asctime(&t1); b = asctime(&t2); CHECK(a != NULL); CHECK_EQ_P(a, b); }
TC_CASE(overwrite) { char *a, *b; mk(&t1, 117, 0, 15, 0, 0, 0, 0); mk(&t2, 120, 1, 29, 1, 2, 3, 6); a = asctime(&t1); CHECK_STR(a, "Sun Jan 15 00:00:00 2017\n"); b = asctime(&t2); CHECK_STR(b, "Sat Feb 29 01:02:03 2020\n"); CHECK_STR(a, "Sat Feb 29 01:02:03 2020\n"); }
int main(void) { tc_begin(); TC_RUN(same_pointer); TC_RUN(overwrite); return tc_end(); }
