/* TC
 * id:    tm_strftime_l_basic
 * area:  time
 * funcs: _strftime_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs strftime, _strftime_l
 * note:  msvcr80 and msvcrt.dll lack the _strftime_l import.
 */
#include "tc.h"
#include <time.h>
#include <locale.h>
#include <wchar.h>
#include <errno.h>
#include <stdlib.h>

static void mk(struct tm *t, int Y, int Mo, int D, int h, int m, int s, int wd, int yd, int dst)
{
    t->tm_year = Y - 1900; t->tm_mon = Mo - 1; t->tm_mday = D; t->tm_hour = h; t->tm_min = m; t->tm_sec = s;
    t->tm_wday = wd; t->tm_yday = yd; t->tm_isdst = dst;
}

TC_CASE(explicit_c) { struct tm t; char a[64], b[64]; _locale_t l; mk(&t, 2021, 3, 9, 14, 5, 7, 2, 67, 0);
    l = _create_locale(LC_ALL, "C"); CHECK(l != NULL); if (!l) return;
    CHECK_EQ_U(_strftime_l(a, 64, "%x|%X", &t, l), strftime(b, 64, "%x|%X", &t)); CHECK_STR(a, b);
    CHECK_STR(a, "03/09/21|14:05:07"); _free_locale(l); }
TC_CASE(null_locale) { struct tm t; char a[64], b[64]; mk(&t, 2021, 3, 9, 14, 5, 7, 2, 67, 0);
    CHECK_EQ_U(_strftime_l(a, 64, "%A %B", &t, NULL), strftime(b, 64, "%A %B", &t)); CHECK_STR(a, b); }
TC_CASE(names) { struct tm t; char a[64]; _locale_t l; mk(&t, 2021, 3, 9, 14, 5, 7, 2, 67, 0);
    l = _create_locale(LC_ALL, "C"); CHECK(l != NULL); if (!l) return;
    CHECK_EQ_U(_strftime_l(a, 64, "%A %B", &t, l), 13); CHECK_STR(a, "Tuesday March"); _free_locale(l); }
int main(void) { tc_begin(); TC_RUN(explicit_c); TC_RUN(null_locale); TC_RUN(names); return tc_end(); }
