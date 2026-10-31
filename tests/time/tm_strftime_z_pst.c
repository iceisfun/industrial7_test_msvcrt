/* TC
 * id:    tm_strftime_z_pst
 * area:  time
 * funcs: strftime _tzset
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   MS docs strftime (%z %Z), _tzset
 * note:  %z/%Z derive from TZ and tm_isdst.
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

static void set_tz(const char *s) { _putenv(s); _tzset(); }

TC_CASE(pst_std) { struct tm t; char b[64]; size_t n; set_tz("TZ=PST8PDT"); mk(&t, 2020, 1, 15, 12, 0, 0, 3, 14, 0);
    n = strftime(b, sizeof b, "%Z %z", &t); CHECK_EQ_U(n, 9); CHECK_STR(b, "PST -0800"); }
TC_CASE(pst_dst) { struct tm t; char b[64]; size_t n; set_tz("TZ=PST8PDT"); mk(&t, 2020, 7, 4, 12, 0, 0, 6, 185, 1);
    n = strftime(b, sizeof b, "%Z %z", &t); CHECK_EQ_U(n, 9); CHECK_STR(b, "PDT -0700"); }
TC_CASE(jst) { struct tm t; char b[64]; size_t n; set_tz("TZ=JST-9"); mk(&t, 2020, 1, 15, 12, 0, 0, 3, 14, 0);
    n = strftime(b, sizeof b, "%z %Z", &t); CHECK_EQ_U(n, 9); CHECK_STR(b, "+0900 JST"); }
int main(void) { tc_begin(); TC_RUN(pst_std); TC_RUN(pst_dst); TC_RUN(jst); set_tz("TZ="); return tc_end(); }
