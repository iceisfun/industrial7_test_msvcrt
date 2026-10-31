/* TC
 * id:    tm_gmtime32_range
 * area:  time
 * funcs: _gmtime32
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _gmtime32
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
TC_CASE(zero) { __time32_t t = 0; volatile __time32_t v = t; t = v; CHK(_gmtime32(&t), 1970, 1, 1, 0, 0, 0, 4, 0); }
TC_CASE(max32) { volatile __time32_t v = 0x7fffffff; __time32_t t = v; CHK(_gmtime32(&t), 2038, 1, 19, 3, 14, 7, 2, 18); }
TC_CASE(day2) { volatile __time32_t v = 86400; __time32_t t = v; CHK(_gmtime32(&t), 1970, 1, 2, 0, 0, 0, 5, 1); }
int main(void) { tc_begin(); TC_RUN(zero); TC_RUN(max32); TC_RUN(day2); return tc_end(); }
