/* TC
 * id:    tm_strftime_week_numbers
 * area:  time
 * funcs: strftime
 * crt:   all
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.27.3.5 (%U week of year, Sunday first; %W Monday first)
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

TC_CASE(sunday_jan1_2017)   { CHECK_STR(fmt("%U", 2017, 1, 1, 0, 0), "01"); CHECK_STR(fmt("%W", 2017, 1, 1, 0, 0), "00"); }
TC_CASE(monday_jan2_2017)   { CHECK_STR(fmt("%U", 2017, 1, 2, 1, 1), "01"); CHECK_STR(fmt("%W", 2017, 1, 2, 1, 1), "01"); }
TC_CASE(saturday_2016_end)  { CHECK_STR(fmt("%U", 2016, 12, 31, 6, 365), "52"); CHECK_STR(fmt("%W", 2016, 12, 31, 6, 365), "52"); }
TC_CASE(monday_jan1_2018)   { CHECK_STR(fmt("%U", 2018, 1, 1, 1, 0), "00"); CHECK_STR(fmt("%W", 2018, 1, 1, 1, 0), "01"); }
int main(void)
{
    tc_begin();
    TC_RUN(sunday_jan1_2017); TC_RUN(monday_jan2_2017); TC_RUN(saturday_2016_end); TC_RUN(monday_jan1_2018);
    return tc_end();
}
