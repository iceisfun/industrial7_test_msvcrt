/* TC
 * id:    tm_neg_wasctime_invalid_fields
 * area:  time
 * kind:  negative
 * funcs: _wasctime
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs asctime, _wasctime (NULL on invalid tm; field ranges per asctime_s)
 * note:  errno==EINVAL and the invalid-parameter handler run for non-_s asctime are the usual secure-CRT validation; msvcrt.dll excluded.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s, int wd)
{
    t->tm_year = y; t->tm_mon = mo; t->tm_mday = d; t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_wday = wd; t->tm_yday = 0; t->tm_isdst = 0;
}
static struct tm t;
static void bad(void) { wchar_t *r; errno = 0; tc_iph_count = 0; r = _wasctime(&t); CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
#define GOOD mk(&t, 117, 9, 23, 14, 5, 9, 1)
TC_CASE(mon) { GOOD; t.tm_mon = 12; bad(); }
TC_CASE(mday) { GOOD; t.tm_mday = 0; bad(); }
TC_CASE(hour) { GOOD; t.tm_hour = 24; bad(); }
TC_CASE(min) { GOOD; t.tm_min = 60; bad(); }
TC_CASE(wday) { GOOD; t.tm_wday = 7; bad(); }
TC_CASE(year) { GOOD; t.tm_year = -1; bad(); }
TC_CASE(recovers) { wchar_t *r; GOOD; t.tm_mon = 12; bad(); GOOD; tc_iph_count = 0; r = _wasctime(&t); CHECK(r != NULL); if (r) CHECK_WSTR(r, L"Mon Oct 23 14:05:09 2017\n"); CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(mon); TC_RUN(mday); TC_RUN(hour); TC_RUN(min); TC_RUN(wday); TC_RUN(year); TC_RUN(recovers); return tc_end(); }
