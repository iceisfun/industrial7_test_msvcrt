/* TC
 * id:    tm_gmtime64_known_dates
 * area:  time
 * funcs: _gmtime64
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _gmtime64 (Unix epoch, UTC); Gregorian calendar arithmetic (values cross-checked independently)
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

TC_CASE(leap_day_2000) { __time64_t t = g(951782400LL); CHK(_gmtime64(&t), 2000, 2, 29, 0, 0, 0, 2, 59); }
TC_CASE(billennium) { __time64_t t = g(1000000000LL); CHK(_gmtime64(&t), 2001, 9, 9, 1, 46, 40, 0, 251); }
TC_CASE(sequence) { __time64_t t = g(1234567890LL); CHK(_gmtime64(&t), 2009, 2, 13, 23, 31, 30, 5, 43); }
TC_CASE(int32_max) { __time64_t t = g(2147483647LL); CHK(_gmtime64(&t), 2038, 1, 19, 3, 14, 7, 2, 18); }
TC_CASE(year_2100) { __time64_t t = g(4102444800LL); CHK(_gmtime64(&t), 2100, 1, 1, 0, 0, 0, 5, 0); }
int main(void) { tc_begin(); TC_RUN(leap_day_2000); TC_RUN(billennium); TC_RUN(sequence); TC_RUN(int32_max); TC_RUN(year_2100); return tc_end(); }
