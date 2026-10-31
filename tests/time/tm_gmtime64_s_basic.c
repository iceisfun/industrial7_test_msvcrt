/* TC
 * id:    tm_gmtime64_s_basic
 * area:  time
 * funcs:
 * procs: _gmtime64_s
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _gmtime64_s
 * note:  Resolved via procs: mingw import libs for some CRTs lack it; real msvcr80+/msvcrt export it.
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
TC_DECL_PROC(int, _gmtime64_s, (struct tm *, const __time64_t *));
TC_CASE(epoch) { struct tm tm; __time64_t t = g(0); CHECK(tc_p__gmtime64_s != NULL); if (!tc_p__gmtime64_s) return; CHECK_EQ_I(tc_p__gmtime64_s(&tm, &t), 0); CHK(&tm, 1970, 1, 1, 0, 0, 0, 4, 0); }
TC_CASE(leap) { struct tm tm; __time64_t t = g(951782400LL); if (!tc_p__gmtime64_s) return; CHECK_EQ_I(tc_p__gmtime64_s(&tm, &t), 0); CHK(&tm, 2000, 2, 29, 0, 0, 0, 2, 59); }
TC_CASE(max) { struct tm tm; __time64_t t = g(32535215999LL); if (!tc_p__gmtime64_s) return; CHECK_EQ_I(tc_p__gmtime64_s(&tm, &t), 0); CHK(&tm, 3000, 12, 31, 23, 59, 59, 3, 364); }
TC_CASE(caller_buffer)
{
    struct tm a, b; __time64_t t1 = g(0), t2 = g(86400);
    if (!tc_p__gmtime64_s) return;
    CHECK_EQ_I(tc_p__gmtime64_s(&a, &t1), 0); CHECK_EQ_I(tc_p__gmtime64_s(&b, &t2), 0);
    CHK(&a, 1970, 1, 1, 0, 0, 0, 4, 0); CHK(&b, 1970, 1, 2, 0, 0, 0, 5, 1);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_gmtime64_s); TC_RUN(epoch); TC_RUN(leap); TC_RUN(max); TC_RUN(caller_buffer); return tc_end(); }
