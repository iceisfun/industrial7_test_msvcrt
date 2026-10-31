/* TC
 * id:    tm_wcsftime_l_basic
 * area:  time
 * funcs: _wcsftime_l _create_locale _free_locale wcsftime
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs strftime, wcsftime, _strftime_l, _wcsftime_l, _create_locale
 */
#include "tc.h"
#include <time.h>
#include <locale.h>
#include <wchar.h>

static struct tm mk(void)
{
    struct tm t; t.tm_year = 120; t.tm_mon = 0; t.tm_mday = 15; t.tm_hour = 12; t.tm_min = 30; t.tm_sec = 45;
    t.tm_wday = 3; t.tm_yday = 14; t.tm_isdst = 0; return t;
}

TC_CASE(c_locale)
{
    struct tm t = mk(); wchar_t b[64]; size_t n; _locale_t l = _create_locale(LC_ALL, "C");
    CHECK(l != NULL);
    n = _wcsftime_l(b, 64, L"%Y-%m-%d %H:%M:%S", &t, l);
    CHECK_EQ_U(n, 19); CHECK_WSTR(b, L"2020-01-15 12:30:45");
    n = _wcsftime_l(b, 64, L"%A %B %p %j", &t, l);
    CHECK_WSTR(b, L"Wednesday January PM 015"); CHECK_EQ_U(n, 24);
    if (l) _free_locale(l);
}
TC_CASE(null_locale)
{
    struct tm t = mk(); wchar_t b[64]; size_t n = _wcsftime_l(b, 64, L"%d/%m/%y", &t, NULL);
    CHECK_EQ_U(n, 8); CHECK_WSTR(b, L"15/01/20");
}
TC_CASE(matches_wcsftime)
{
    struct tm t = mk(); wchar_t a[64], b[64]; size_t n1, n2; _locale_t l = _create_locale(LC_ALL, "C");
    n1 = _wcsftime_l(a, 64, L"%a %b %d %H:%M:%S %Y", &t, l);
    n2 = wcsftime(b, 64, L"%a %b %d %H:%M:%S %Y", &t);
    CHECK_EQ_U(n1, n2); CHECK_WSTR(a, b); CHECK_WSTR(a, L"Wed Jan 15 12:30:45 2020");
    if (l) _free_locale(l);
}
int main(void) { tc_begin(); TC_RUN(c_locale); TC_RUN(null_locale); TC_RUN(matches_wcsftime); return tc_end(); }
