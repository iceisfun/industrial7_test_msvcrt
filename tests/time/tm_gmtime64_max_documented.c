/* TC
 * id:    tm_gmtime64_max_documented
 * area:  time
 * funcs: _gmtime64
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _gmtime64: range ends 23:59:59 Dec 31 3000 UTC
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
TC_CASE(max) { __time64_t t = g(32535215999LL); CHK(_gmtime64(&t), 3000, 12, 31, 23, 59, 59, 3, 364); }
TC_CASE(max_minus_1) { __time64_t t = g(32535215998LL); CHK(_gmtime64(&t), 3000, 12, 31, 23, 59, 58, 3, 364); }

int main(void) { tc_begin(); TC_RUN(max); TC_RUN(max_minus_1); return tc_end(); }
