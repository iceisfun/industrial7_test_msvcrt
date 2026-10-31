/* TC
 * id:    tm_gmtime64_epoch
 * area:  time
 * funcs: _gmtime64
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _gmtime64
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

TC_CASE(epoch) { __time64_t t = g(0); CHK(_gmtime64(&t), 1970, 1, 1, 0, 0, 0, 4, 0); }
TC_CASE(end_of_day) { __time64_t t = g(86399); CHK(_gmtime64(&t), 1970, 1, 1, 23, 59, 59, 4, 0); }
TC_CASE(next_day) { __time64_t t = g(86400); CHK(_gmtime64(&t), 1970, 1, 2, 0, 0, 0, 5, 1); }
TC_CASE(one_second) { __time64_t t = g(1); CHK(_gmtime64(&t), 1970, 1, 1, 0, 0, 1, 4, 0); }
int main(void) { tc_begin(); TC_RUN(epoch); TC_RUN(end_of_day); TC_RUN(next_day); TC_RUN(one_second); return tc_end(); }
