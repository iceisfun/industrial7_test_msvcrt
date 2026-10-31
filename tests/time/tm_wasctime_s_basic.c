/* TC
 * id:    tm_wasctime_s_basic
 * area:  time
 * funcs: 
 * procs: _wasctime_s
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs asctime_s, _wasctime_s
 * note:  msvcrt.dll excluded: unsure Win7 msvcrt.dll exports the _s time functions; _wasctime_s success
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s, int wd)
{
    volatile int *p; t->tm_year = y; t->tm_mon = mo; t->tm_mday = d; t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_wday = wd; t->tm_yday = 0; t->tm_isdst = 0; (void)p;
}
TC_DECL_PROC(int, _wasctime_s, (wchar_t *, size_t, const struct tm *));
static struct tm t;
TC_CASE(exact26) { wchar_t b[26]; mk(&t, 117, 9, 23, 14, 5, 9, 1); CHECK_EQ_I(tc_p__wasctime_s(b, 26, &t), 0); CHECK_WSTR(b, L"Mon Oct 23 14:05:09 2017\n"); CHECK_EQ_I(b[25], 0); }
TC_CASE(bigger) { wchar_t b[64]; mk(&t, 100, 0, 15, 0, 0, 0, 6); CHECK_EQ_I(tc_p__wasctime_s(b, 64, &t), 0); CHECK_WSTR(b, L"Sat Jan 15 00:00:00 2000\n"); }
int main(void) { tc_begin(); TC_LOAD_PROC(_wasctime_s); CHECK(tc_p__wasctime_s); if (!tc_p__wasctime_s) return tc_end(); TC_RUN(exact26); TC_RUN(bigger); return tc_end(); }
