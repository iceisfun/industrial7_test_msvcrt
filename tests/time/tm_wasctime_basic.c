/* TC
 * id:    tm_wasctime_basic
 * area:  time
 * funcs: _wasctime
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs asctime, _wasctime
 * note:  wide asctime
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s, int wd)
{
    volatile int *p; t->tm_year = y; t->tm_mon = mo; t->tm_mday = d; t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_wday = wd; t->tm_yday = 0; t->tm_isdst = 0; (void)p;
}
static struct tm t;
TC_CASE(text) { mk(&t, 117, 9, 23, 14, 5, 9, 1); CHECK_WSTR(_wasctime(&t), L"Mon Oct 23 14:05:09 2017\n"); }
TC_CASE(length) { wchar_t *s; size_t n = 0; mk(&t, 117, 0, 15, 0, 0, 0, 0); s = _wasctime(&t); CHECK(s != NULL); if (s) { while (s[n]) n++; CHECK_EQ_U(n, 25); } }
TC_CASE(y2000) { mk(&t, 100, 0, 15, 0, 0, 0, 6); CHECK_WSTR(_wasctime(&t), L"Sat Jan 15 00:00:00 2000\n"); }
int main(void) { tc_begin(); TC_RUN(text); TC_RUN(length); TC_RUN(y2000); return tc_end(); }
