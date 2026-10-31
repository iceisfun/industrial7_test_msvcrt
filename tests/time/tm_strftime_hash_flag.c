/* TC
 * id:    tm_strftime_hash_flag
 * area:  time
 * funcs: strftime
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs strftime: "#" flag removes leading zeros (d H I j m M S U w W y Y)
 */
#include "tc.h"
#include <time.h>

static const char *fmt(const char *f)
{
    static char buf[96]; volatile const char *vf = f; struct tm t; char *p = (char *)&t; size_t i;
    for (i = 0; i < sizeof t; i++) p[i] = 0;
    t.tm_year = 117; t.tm_mon = 2; t.tm_mday = 5; t.tm_hour = 7; t.tm_min = 8; t.tm_sec = 9; t.tm_wday = 0; t.tm_yday = 63;
    if (strftime(buf, sizeof buf, (const char *)vf, &t) == 0) return "(zero)";
    return buf;
}
TC_CASE(day_hour) { CHECK_STR(fmt("%#d"), "5"); CHECK_STR(fmt("%#H"), "7"); CHECK_STR(fmt("%#I"), "7"); }
TC_CASE(yday_mon) { CHECK_STR(fmt("%#j"), "64"); CHECK_STR(fmt("%#m"), "3"); }
TC_CASE(min_sec_year) { CHECK_STR(fmt("%#M"), "8"); CHECK_STR(fmt("%#S"), "9"); CHECK_STR(fmt("%#y"), "17"); }
int main(void)
{
    tc_begin();
    TC_RUN(day_hour); TC_RUN(yday_mon); TC_RUN(min_sec_year);
    return tc_end();
}
