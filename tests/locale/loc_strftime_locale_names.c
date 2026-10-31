/* TC
 * id:    loc_strftime_locale_names
 * area:  locale
 * funcs: strftime setlocale
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs strftime (names depend on LC_TIME); C11 7.27.3.5
 * note:  Fixed time Sunday 2024-01-07 09:05:03. German names Sonntag / Januar come from NLS; %a %b %p are not asserted for German.
 */
#include "tc.h"
#include <locale.h>
#include <time.h>

static struct tm mk(void)
{
    struct tm t; char *p = (char *)&t; size_t i;
    for (i = 0; i < sizeof t; i++) p[i] = 0;
    t.tm_year = 124; t.tm_mon = 0; t.tm_mday = 7; t.tm_wday = 0; t.tm_yday = 6;
    t.tm_hour = 9; t.tm_min = 5; t.tm_sec = 3;
    return t;
}
static volatile const char *v_fmt;
static int fmt(char *buf, size_t n, const char *f)
{
    struct tm t = mk(); v_fmt = f;
    return (int)strftime(buf, n, (const char *)v_fmt, &t);
}
static int enter(const char *n) { volatile const char *v = n; return setlocale(LC_ALL, (const char *)v) != NULL; }

TC_CASE(c_locale_names)
{
    char b[64];
    setlocale(LC_ALL, "C");
    CHECK(fmt(b, sizeof b, "%A %B %a %b") > 0);
    CHECK_STR(b, "Sunday January Sun Jan");
}
TC_CASE(english_names)
{
    char b[64];
    if (!enter("English_United States.1252")) { setlocale(LC_ALL, "C"); TC_SKIP("English locale unavailable"); }
    CHECK(fmt(b, sizeof b, "%A %B %a %b") > 0);
    CHECK_STR(b, "Sunday January Sun Jan");
    setlocale(LC_ALL, "C");
}
TC_CASE(german_names)
{
    char b[64];
    if (!enter("German_Germany.1252")) { setlocale(LC_ALL, "C"); TC_SKIP("German locale unavailable"); }
    CHECK(fmt(b, sizeof b, "%A") > 0);
    CHECK_STR(b, "Sonntag");
    CHECK(fmt(b, sizeof b, "%B") > 0);
    CHECK_STR(b, "Januar");
    setlocale(LC_ALL, "C");
}
TC_CASE(ampm)
{
    char b[64];
    setlocale(LC_ALL, "C");
    CHECK(fmt(b, sizeof b, "%p") > 0);
    CHECK_STR(b, "AM");
    if (enter("English_United States.1252")) { CHECK(fmt(b, sizeof b, "%p") > 0); CHECK_STR(b, "AM"); }
    setlocale(LC_ALL, "C");
}
TC_CASE(restore_c)
{
    char b[64];
    setlocale(LC_ALL, "C");
    CHECK_STR(setlocale(LC_ALL, NULL), "C");
    CHECK(fmt(b, sizeof b, "%A") > 0);
    CHECK_STR(b, "Sunday");
}
int main(void)
{
    tc_begin();
    TC_RUN(c_locale_names); TC_RUN(english_names); TC_RUN(german_names); TC_RUN(ampm); TC_RUN(restore_c);
    return tc_end();
}
