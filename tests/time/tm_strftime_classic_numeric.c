/* TC
 * id:    tm_strftime_classic_numeric
 * area:  time
 * funcs: strftime
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.27.3.5 strftime; MS docs strftime. %j is 1-based (tm_yday + 1)
 */
#include "tc.h"
#include <time.h>

static struct tm mk(int y, int mo, int d, int h, int mi, int s, int wd, int yd)
{
    struct tm t; char *p = (char *)&t; size_t i;
    for (i = 0; i < sizeof t; i++) p[i] = 0;
    t.tm_year = y - 1900; t.tm_mon = mo - 1; t.tm_mday = d; t.tm_hour = h; t.tm_min = mi; t.tm_sec = s;
    t.tm_wday = wd; t.tm_yday = yd;
    return t;
}
static const char *fmt(const char *f, const struct tm *t)
{
    static char buf[128]; volatile const char *vf = f; size_t n;
    buf[0] = 'X';
    n = strftime(buf, sizeof buf, (const char *)vf, t);
    if (n == 0) return "(zero)";
    return buf;
}

TC_CASE(fields)
{
    struct tm t = mk(2017, 3, 5, 7, 8, 9, 0, 63);
    CHECK_STR(fmt("%d", &t), "05");
    CHECK_STR(fmt("%H", &t), "07");
    CHECK_STR(fmt("%I", &t), "07");
    CHECK_STR(fmt("%j", &t), "064");
    CHECK_STR(fmt("%m", &t), "03");
    CHECK_STR(fmt("%M", &t), "08");
    CHECK_STR(fmt("%S", &t), "09");
    CHECK_STR(fmt("%y", &t), "17");
    CHECK_STR(fmt("%Y", &t), "2017");
    CHECK_STR(fmt("%w", &t), "0");
    CHECK_STR(fmt("%Y-%m-%d %H:%M:%S", &t), "2017-03-05 07:08:09");
}
TC_CASE(hour12)
{
    struct tm t = mk(2017, 3, 5, 0, 8, 9, 0, 63);
    CHECK_STR(fmt("%I", &t), "12");
    t.tm_hour = 12; CHECK_STR(fmt("%I", &t), "12");
    t.tm_hour = 13; CHECK_STR(fmt("%I", &t), "01");
    t.tm_hour = 23; CHECK_STR(fmt("%I", &t), "11");
}
TC_CASE(yday_range)
{
    struct tm t = mk(2016, 1, 1, 0, 0, 0, 5, 0);
    CHECK_STR(fmt("%j", &t), "001");
    t = mk(2016, 12, 31, 0, 0, 0, 6, 365);
    CHECK_STR(fmt("%j", &t), "366");
}
TC_CASE(percent)
{
    struct tm t = mk(2017, 3, 5, 7, 8, 9, 0, 63);
    CHECK_STR(fmt("%%", &t), "%");
    CHECK_STR(fmt("100%%", &t), "100%");
}
int main(void)
{
    tc_begin();
    TC_RUN(fields); TC_RUN(hour12); TC_RUN(yday_range); TC_RUN(percent);
    return tc_end();
}
