/* TC
 * id:    tm_wcsftime_basic
 * area:  time
 * funcs: wcsftime
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.5.1 wcsftime; MS docs wcsftime
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
TC_CASE(numeric) { wchar_t b[64]; mk(&T, 2021, 3, 9, 14, 5, 7, 2, 67, 0);
    CHECK_EQ_U(wcsftime(b, 64, L"%Y-%m-%d %H:%M:%S", &T), 19); CHECK_WSTR(b, L"2021-03-09 14:05:07"); }
TC_CASE(names) { wchar_t b[64]; mk(&T, 2021, 3, 9, 14, 5, 7, 2, 67, 0);
    CHECK_EQ_U(wcsftime(b, 64, L"%a %A %b %B %p", &T), 24); CHECK_WSTR(b, L"Tue Tuesday Mar March PM"); }
TC_CASE(misc) { wchar_t b[64]; mk(&T, 2021, 3, 9, 14, 5, 7, 2, 67, 0);
    CHECK_EQ_U(wcsftime(b, 64, L"%j %U %W %w %y %%", &T), 16); CHECK_WSTR(b, L"068 10 10 2 21 %"); }
TC_CASE(fit) { wchar_t b[11]; mk(&T, 2021, 3, 9, 14, 5, 7, 2, 67, 0); b[10] = L'x';
    CHECK_EQ_U(wcsftime(b, 11, L"%Y-%m-%d", &T), 10); CHECK_EQ_I(b[10], 0); CHECK_WSTR(b, L"2021-03-09"); }
TC_CASE(locale_c_xX) { wchar_t b[64]; mk(&T, 2021, 3, 9, 14, 5, 7, 2, 67, 0);
    CHECK_EQ_U(wcsftime(b, 64, L"%x", &T), 8); CHECK_WSTR(b, L"03/09/21");
    CHECK_EQ_U(wcsftime(b, 64, L"%X", &T), 8); CHECK_WSTR(b, L"14:05:07"); }
int main(void) { tc_begin(); TC_RUN(numeric); TC_RUN(names); TC_RUN(misc); TC_RUN(fit); TC_RUN(locale_c_xX); return tc_end(); }
