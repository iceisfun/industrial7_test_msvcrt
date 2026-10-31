/* TC
 * id:    tm_gmtime64_leap_years
 * area:  time
 * funcs: _gmtime64
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   Gregorian leap rules; MS docs _gmtime64 (Unix epoch, UTC)
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

TC_CASE(y1972) { __time64_t t = g(68169600LL); CHK(_gmtime64(&t), 1972, 2, 29, 0, 0, 0, 2, 59); }
TC_CASE(y2000_end) { __time64_t t = g(978307199LL); CHK(_gmtime64(&t), 2000, 12, 31, 23, 59, 59, 0, 365); }
TC_CASE(y2100_not_leap) { __time64_t t = g(4107456000LL + 0); CHK(_gmtime64(&t), 2100, 2, 28, 0, 0, 0, 0, 58);
    t += 86400; CHK(_gmtime64(&t), 2100, 3, 1, 0, 0, 0, 1, 59); }
TC_CASE(y2096_end) { __time64_t t = g(4007836799LL); CHK(_gmtime64(&t), 2096, 12, 31, 23, 59, 59, 1, 365); }
TC_CASE(y1999_end) { __time64_t t = g(946684799LL); CHK(_gmtime64(&t), 1999, 12, 31, 23, 59, 59, 5, 364); }
int main(void) { tc_begin(); TC_RUN(y1972); TC_RUN(y2000_end); TC_RUN(y2100_not_leap); TC_RUN(y2096_end); TC_RUN(y1999_end); return tc_end(); }
