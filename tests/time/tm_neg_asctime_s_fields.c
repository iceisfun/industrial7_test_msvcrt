/* TC
 * id:    tm_neg_asctime_s_fields
 * area:  time
 * funcs: 
 * procs: asctime_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs asctime_s error table; tm_year 0..8099
 * note: msvcrt.dll excluded: invalid-parameter handler support there is not assured
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
static void bad(void) { char b[26]; int r; tc_iph_count = 0; r = tc_p_asctime_s(b, 26, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
#define GOOD mk(&t, 117, 9, 23, 14, 5, 9, 1)
TC_CASE(year) { GOOD; t.tm_year = -1; bad(); GOOD }
TC_CASE(mon) { GOOD; t.tm_mon = 12; bad(); }
TC_CASE(mday) { GOOD; t.tm_mday = 0; bad(); }
TC_CASE(hour) { GOOD; t.tm_hour = 24; bad(); }
TC_CASE(min) { GOOD; t.tm_min = 60; bad(); }
TC_CASE(wday) { GOOD; t.tm_wday = 7; bad(); }
TC_CASE(boundary_ok) { char b[26]; mk(&t, 117, 11, 31, 23, 59, 59, 0); tc_iph_count = 0; CHECK_EQ_I(tc_p_asctime_s(b, 26, &t), 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(b, "Sun Dec 31 23:59:59 2017\n"); }
int main(void) { tc_begin(); tc_iph_install(); TC_LOAD_PROC(asctime_s); CHECK(tc_p_asctime_s); if (!tc_p_asctime_s) return tc_end(); TC_RUN(year); TC_RUN(mon); TC_RUN(mday); TC_RUN(hour); TC_RUN(min); TC_RUN(wday); TC_RUN(boundary_ok); return tc_end(); }
