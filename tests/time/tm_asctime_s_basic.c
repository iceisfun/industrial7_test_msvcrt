/* TC
 * id:    tm_asctime_s_basic
 * area:  time
 * funcs: 
 * procs: asctime_s
 * crt:   >=90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs asctime_s, _wasctime_s
 * note:  msvcrt.dll excluded: unsure Win7 msvcrt.dll exports the _s time functions; asctime_s success
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s, int wd)
{
    volatile int *p; t->tm_year = y; t->tm_mon = mo; t->tm_mday = d; t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_wday = wd; t->tm_yday = 0; t->tm_isdst = 0; (void)p;
}
TC_DECL_PROC(int, asctime_s, (char *, size_t, const struct tm *));
static struct tm t;
TC_CASE(exact26) { char b[26]; int r; mk(&t, 117, 9, 23, 14, 5, 9, 1); r = tc_p_asctime_s(b, 26, &t); CHECK_EQ_I(r, 0); CHECK_STR(b, "Mon Oct 23 14:05:09 2017\n"); CHECK_EQ_I(b[24], '\n'); CHECK_EQ_I(b[25], 0); }
TC_CASE(bigger) { char b[64]; int i, r; for (i = 0; i < 64; i++) b[i] = 'x'; mk(&t, 117, 0, 15, 0, 0, 0, 0); r = tc_p_asctime_s(b, 64, &t); CHECK_EQ_I(r, 0); CHECK_EQ_I(b[25], 0); CHECK_STR(b, "Sun Jan 15 00:00:00 2017\n"); }
TC_CASE(names)
{
    static const char *w[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    static const char *m[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    int i;
    for (i = 0; i < 12; i++) { char b[26]; mk(&t, 117, i, 1, 0, 0, 0, i % 7); CHECK_EQ_I(tc_p_asctime_s(b, 26, &t), 0); CHECK(b[0] == w[i % 7][0] && b[1] == w[i % 7][1] && b[2] == w[i % 7][2]); CHECK(b[4] == m[i][0] && b[5] == m[i][1] && b[6] == m[i][2]); }
}
int main(void) { tc_begin(); TC_LOAD_PROC(asctime_s); CHECK(tc_p_asctime_s); if (!tc_p_asctime_s) return tc_end(); TC_RUN(exact26); TC_RUN(bigger); TC_RUN(names); return tc_end(); }
