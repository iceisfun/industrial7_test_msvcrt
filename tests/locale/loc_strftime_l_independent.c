/* TC
 * id:    loc_strftime_l_independent
 * area:  locale
 * funcs: _strftime_l _create_locale _free_locale setlocale
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _strftime_l, _create_locale
 * note:  Global locale stays "C" while the locale object supplies the names. Sunday 2024-01-07 09:05:03.
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
static volatile const char *v_name;
static volatile const char *v_fmt = "%A %B";

TC_CASE(german_object)
{
    char b[64]; struct tm t = mk(); _locale_t l;
    setlocale(LC_ALL, "C");
    v_name = "German_Germany.1252";
    l = _create_locale(LC_ALL, (const char *)v_name);
    if (!l) TC_SKIP("German locale unavailable");
    CHECK(_strftime_l(b, sizeof b, (const char *)v_fmt, &t, l) > 0);
    CHECK_STR(b, "Sonntag Januar");
    _free_locale(l);
}
TC_CASE(c_object)
{
    char b[64]; struct tm t = mk(); _locale_t l;
    v_name = "C";
    l = _create_locale(LC_ALL, (const char *)v_name);
    CHECK(l != NULL);
    if (!l) return;
    CHECK(_strftime_l(b, sizeof b, (const char *)v_fmt, &t, l) > 0);
    CHECK_STR(b, "Sunday January");
    _free_locale(l);
}
TC_CASE(global_unchanged)
{
    char b[64]; struct tm t = mk(); _locale_t l;
    v_name = "German_Germany.1252";
    l = _create_locale(LC_ALL, (const char *)v_name);
    if (!l) TC_SKIP("German locale unavailable");
    CHECK_STR(setlocale(LC_ALL, NULL), "C");
    CHECK(strftime(b, sizeof b, (const char *)v_fmt, &t) > 0);
    CHECK_STR(b, "Sunday January");
    _free_locale(l);
}
int main(void) { tc_begin(); TC_RUN(german_object); TC_RUN(c_object); TC_RUN(global_unchanged); return tc_end(); }
