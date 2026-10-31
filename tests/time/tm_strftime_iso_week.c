/* TC
 * id:    tm_strftime_iso_week
 * area:  time
 * funcs: strftime
 * crt:   ucrtbase
 * tier:  p1
 * conf:  standard
 * ref:   C99 7.23.3.5 / C11 7.27.3.5 (%G %g %V ISO 8601 week-based year)
 */
#include "tc.h"
#include <time.h>

static const char *fmt(const char *f, int y, int mo, int d, int wd, int yd)
{
    static char buf[64]; volatile const char *vf = f; struct tm t; char *p = (char *)&t; size_t i;
    for (i = 0; i < sizeof t; i++) p[i] = 0;
    t.tm_year = y - 1900; t.tm_mon = mo - 1; t.tm_mday = d; t.tm_wday = wd; t.tm_yday = yd;
    if (strftime(buf, sizeof buf, (const char *)vf, &t) == 0) return "(zero)";
    return buf;
}
TC_CASE(dec29_2014)
{
    CHECK_STR(fmt("%G", 2014, 12, 29, 1, 362), "2015");
    CHECK_STR(fmt("%g", 2014, 12, 29, 1, 362), "15");
    CHECK_STR(fmt("%V", 2014, 12, 29, 1, 362), "01");
}
TC_CASE(jan1_2016)
{
    CHECK_STR(fmt("%G", 2016, 1, 1, 5, 0), "2015");
    CHECK_STR(fmt("%V", 2016, 1, 1, 5, 0), "53");
}
TC_CASE(dec31_2018)
{
    CHECK_STR(fmt("%G", 2018, 12, 31, 1, 364), "2019");
    CHECK_STR(fmt("%V", 2018, 12, 31, 1, 364), "01");
}
TC_CASE(jan2_2017) { CHECK_STR(fmt("%V", 2017, 1, 2, 1, 1), "01"); }
int main(void)
{
    tc_begin();
    TC_RUN(dec29_2014); TC_RUN(jan1_2016); TC_RUN(dec31_2018); TC_RUN(jan2_2017);
    return tc_end();
}
