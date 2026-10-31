/* TC
 * id:    tm_wcsftime_c99_specifiers
 * area:  time
 * funcs: wcsftime
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs wcsftime (UCRT specifiers)
 */
#include "tc.h"
#include <time.h>
#include <wchar.h>
#include <errno.h>
#include <stdlib.h>

static void mk(struct tm *t, int Y, int Mo, int D, int h, int m, int s, int wd, int yd, int dst)
{
    t->tm_year = Y - 1900; t->tm_mon = Mo - 1; t->tm_mday = D; t->tm_hour = h; t->tm_min = m; t->tm_sec = s;
    t->tm_wday = wd; t->tm_yday = yd; t->tm_isdst = dst;
}

static struct tm T;
static void f(const wchar_t *fmt, const wchar_t *want) { wchar_t b[96]; size_t n = wcsftime(b, 96, fmt, &T); CHECK_EQ_U(n, wcslen(want)); CHECK_WSTR(b, want); }
TC_CASE(group1) { mk(&T, 2021, 3, 9, 14, 5, 7, 2, 67, 0);
    f(L"%F", L"2021-03-09"); f(L"%T", L"14:05:07"); f(L"%D", L"03/09/21"); f(L"%e", L" 9");
    f(L"%R", L"14:05"); f(L"%r", L"02:05:07 PM"); }
TC_CASE(group2) { mk(&T, 2021, 3, 9, 14, 5, 7, 2, 67, 0);
    f(L"%C", L"20"); f(L"%u", L"2"); f(L"%V", L"10"); f(L"%G", L"2021"); f(L"%g", L"21"); f(L"%h", L"Mar"); }
TC_CASE(hash_d) { mk(&T, 2021, 3, 9, 14, 5, 7, 2, 67, 0); f(L"%#d", L"9"); f(L"%#H", L"14"); f(L"%#j", L"68"); }
int main(void) { tc_begin(); TC_RUN(group1); TC_RUN(group2); TC_RUN(hash_d); return tc_end(); }
