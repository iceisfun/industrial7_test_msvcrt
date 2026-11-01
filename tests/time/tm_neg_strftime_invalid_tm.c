/* TC
 * id:    tm_neg_strftime_invalid_tm
 * area:  time
 * funcs: strftime
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs strftime: tm fields are validated
 * note:  Each field is checked in its own case, with a format that uses it.
 *        Win10 19045 ucrtbase: handler count is 2 per failing strftime call with an invalid tm field (errno EINVAL, return 0 as before); an invalid format specifier counts 1.
 */
#include "tc.h"
#include <time.h>
#include <wchar.h>
#include <errno.h>
#include <stdlib.h>

static void mk(struct tm *t, int Y, int Mo, int D, int h, int m, int s, int wd, int yd, int dst)
{
    t->tm_year = Y - 1900; t->tm_mon = Mo - 1; t->tm_mday = D; t->tm_hour = h; t->tm_min = m; t->tm_sec = s;
    t->tm_wday = wd; t->tm_yday = yd; t->tm_isdst = dst;
}

/* observed on Win10 ucrtbase: an invalid tm field invokes the invalid-parameter handler TWICE per strftime call (once for the tm, once more from the format loop) */
#if TC_CRT_VER >= 140
#define IPH_N 2
#else
#define IPH_N 1
#endif
static char buf[64];
static void bad(struct tm *t, const char *fmt)
{
    size_t r; errno = 0; tc_iph_count = 0; buf[0] = 0;
    r = strftime(buf, sizeof buf, fmt, t);
    CHECK_EQ_U(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, IPH_N);
}
TC_CASE(mon12) { struct tm t; mk(&t, 2020, 6, 15, 12, 30, 30, 1, 166, 0); t.tm_mon = 12; bad(&t, "%b"); t.tm_mon = -1; bad(&t, "%b"); }
TC_CASE(wday7) { struct tm t; mk(&t, 2020, 6, 15, 12, 30, 30, 1, 166, 0); t.tm_wday = 7; bad(&t, "%a"); }
TC_CASE(hour24) { struct tm t; mk(&t, 2020, 6, 15, 12, 30, 30, 1, 166, 0); t.tm_hour = 24; bad(&t, "%H"); }
TC_CASE(mday0) { struct tm t; mk(&t, 2020, 6, 15, 12, 30, 30, 1, 166, 0); t.tm_mday = 0; bad(&t, "%d"); }
TC_CASE(mday32) { struct tm t; mk(&t, 2020, 6, 15, 12, 30, 30, 1, 166, 0); t.tm_mday = 32; bad(&t, "%d"); }
TC_CASE(yday_neg) { struct tm t; mk(&t, 2020, 6, 15, 12, 30, 30, 1, 166, 0); t.tm_yday = -1; bad(&t, "%j"); }
TC_CASE(yday366) { struct tm t; mk(&t, 2020, 6, 15, 12, 30, 30, 1, 166, 0); t.tm_yday = 366; bad(&t, "%j"); }
TC_CASE(min60) { struct tm t; mk(&t, 2020, 6, 15, 12, 30, 30, 1, 166, 0); t.tm_min = 60; bad(&t, "%M"); }
int main(void) { tc_begin(); CHECK(tc_iph_install());
    TC_RUN(mon12); TC_RUN(wday7); TC_RUN(hour24); TC_RUN(mday0); TC_RUN(mday32);
    TC_RUN(yday_neg); TC_RUN(yday366); TC_RUN(min60); return tc_end(); }
