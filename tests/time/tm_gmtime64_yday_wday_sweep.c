/* TC
 * id:    tm_gmtime64_yday_wday_sweep
 * area:  time
 * funcs: _gmtime64
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _gmtime64 (Unix epoch, UTC); Gregorian calendar
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>

static void chk(const struct tm *t, int y, int mo, int d, int h, int mi, int s, int wd, int yd, int line)
{
    if (!t) { tc_fail_expr(__FILE__, line, "tm is NULL"); return; }
    if (t->tm_year != y - 1900) tc_fail_ints(__FILE__, line, "tm_year", "y-1900", t->tm_year, y - 1900);
    if (t->tm_mon != mo - 1) tc_fail_ints(__FILE__, line, "tm_mon", "mo-1", t->tm_mon, mo - 1);
    if (t->tm_mday != d) tc_fail_ints(__FILE__, line, "tm_mday", "d", t->tm_mday, d);
    if (t->tm_hour != h) tc_fail_ints(__FILE__, line, "tm_hour", "h", t->tm_hour, h);
    if (t->tm_min != mi) tc_fail_ints(__FILE__, line, "tm_min", "mi", t->tm_min, mi);
    if (t->tm_sec != s) tc_fail_ints(__FILE__, line, "tm_sec", "s", t->tm_sec, s);
    if (wd >= 0 && t->tm_wday != wd) tc_fail_ints(__FILE__, line, "tm_wday", "wd", t->tm_wday, wd);
    if (yd >= 0 && t->tm_yday != yd) tc_fail_ints(__FILE__, line, "tm_yday", "yd", t->tm_yday, yd);
    if (t->tm_isdst != 0) tc_fail_ints(__FILE__, line, "tm_isdst", "0", t->tm_isdst, 0);
}
#define CHK(t, ...) chk(t, __VA_ARGS__, __LINE__)
static __time64_t g(__time64_t v) { volatile __time64_t x = v; return x; }
TC_CASE(sweep)
{
    int i, wday = 4, yday = 0, year = 1970, mon = 0, mday = 1, bad = 0;
    for (i = 0; i <= 1500; i++) {
        __time64_t t = g((__time64_t)i * 86400);
        struct tm *p = _gmtime64(&t);
        int leap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        static const int ml[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
        if (!p) { bad++; break; }
        if (p->tm_wday != wday || p->tm_yday != yday || p->tm_year != year - 1900 || p->tm_mon != mon || p->tm_mday != mday ||
            p->tm_hour || p->tm_min || p->tm_sec || p->tm_isdst) { tc_fail_ints(__FILE__, __LINE__, "sweep day", "ok", i, -1); bad++; break; }
        wday = (wday + 1) % 7; yday++; mday++;
        if (mday > ml[mon] + (mon == 1 && leap)) { mday = 1; mon++; }
        if (mon == 12) { mon = 0; yday = 0; year++; }
    }
    CHECK_EQ_I(bad, 0);
}
int main(void) { tc_begin(); TC_RUN(sweep); return tc_end(); }
