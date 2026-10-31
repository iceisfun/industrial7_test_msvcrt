/* TC
 * id:    tm_strftime_names_c_locale
 * area:  time
 * funcs: strftime
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.27.3.5 (%a %A %b %B %p in the "C" locale)
 */
#include "tc.h"
#include <time.h>

static const char *fmt(const char *f, const struct tm *t)
{
    static char buf[128]; volatile const char *vf = f;
    if (strftime(buf, sizeof buf, (const char *)vf, t) == 0) return "(zero)";
    return buf;
}
static struct tm zt(void)
{
    struct tm t; char *p = (char *)&t; size_t i;
    for (i = 0; i < sizeof t; i++) p[i] = 0;
    t.tm_year = 117; t.tm_mday = 1;
    return t;
}
static const char *const wdab[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char *const wdfu[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
static const char *const moab[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
static const char *const mofu[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

TC_CASE(weekdays)
{
    struct tm t = zt(); int i;
    for (i = 0; i < 7; i++) {
        t.tm_wday = i;
        CHECK_STR(fmt("%a", &t), wdab[i]);
        CHECK_STR(fmt("%A", &t), wdfu[i]);
    }
}
TC_CASE(months)
{
    struct tm t = zt(); int i;
    for (i = 0; i < 12; i++) {
        t.tm_mon = i;
        CHECK_STR(fmt("%b", &t), moab[i]);
        CHECK_STR(fmt("%B", &t), mofu[i]);
    }
}
TC_CASE(am_pm)
{
    struct tm t = zt(); int h;
    for (h = 0; h < 24; h++) {
        t.tm_hour = h;
        CHECK_STR(fmt("%p", &t), h < 12 ? "AM" : "PM");
    }
}
TC_CASE(may_short_equals_full)
{
    struct tm t = zt(); t.tm_mon = 4;
    CHECK_STR(fmt("%b", &t), "May");
    CHECK_STR(fmt("%B", &t), "May");
}
int main(void)
{
    tc_begin();
    TC_RUN(weekdays); TC_RUN(months); TC_RUN(am_pm); TC_RUN(may_short_equals_full);
    return tc_end();
}
