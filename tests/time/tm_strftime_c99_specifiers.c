/* TC
 * id:    tm_strftime_c99_specifiers
 * area:  time
 * funcs: strftime
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strftime (%C %D %e %F %h %n %r %R %t %T %u listed; UCRT / VS2015+)
 */
#include "tc.h"
#include <time.h>

static struct tm base(int wd)
{
    struct tm t; char *p = (char *)&t; size_t i;
    for (i = 0; i < sizeof t; i++) p[i] = 0;
    t.tm_year = 117; t.tm_mon = 2; t.tm_mday = 5; t.tm_hour = 7; t.tm_min = 8; t.tm_sec = 9; t.tm_wday = wd; t.tm_yday = 63;
    return t;
}
static const char *fmt(const char *f, const struct tm *t)
{
    static char buf[64]; volatile const char *vf = f;
    if (strftime(buf, sizeof buf, (const char *)vf, t) == 0) return "(zero)";
    return buf;
}
TC_CASE(date_forms)
{
    struct tm t = base(0);
    CHECK_STR(fmt("%C", &t), "20");
    CHECK_STR(fmt("%D", &t), "03/05/17");
    CHECK_STR(fmt("%e", &t), " 5");
    CHECK_STR(fmt("%F", &t), "2017-03-05");
    CHECK_STR(fmt("%h", &t), "Mar");
}
TC_CASE(time_forms)
{
    struct tm t = base(0);
    CHECK_STR(fmt("%T", &t), "07:08:09");
    CHECK_STR(fmt("%R", &t), "07:08");
    CHECK_STR(fmt("%r", &t), "07:08:09 AM");
}
TC_CASE(whitespace)
{
    struct tm t = base(0);
    CHECK_STR(fmt("a%nb", &t), "a\nb");
    CHECK_STR(fmt("a%tb", &t), "a\tb");
}
TC_CASE(weekday_u)
{
    struct tm t = base(0);
    CHECK_STR(fmt("%u", &t), "7");
    t.tm_wday = 1; CHECK_STR(fmt("%u", &t), "1");
    t.tm_wday = 6; CHECK_STR(fmt("%u", &t), "6");
}
int main(void)
{
    tc_begin();
    TC_RUN(date_forms); TC_RUN(time_forms); TC_RUN(whitespace); TC_RUN(weekday_u);
    return tc_end();
}
