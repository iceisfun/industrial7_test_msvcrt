/* TC
 * id:    tm_ctime_probe_day_padding
 * area:  time
 * funcs: _ctime64 _wasctime
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.27.3.1 (asctime pads day with a space); MS docs asctime/ctime examples show "Wed Jan 02 02:03:55 1980"
 * note:  PROBE: padding of a single-digit day of month. The C standard says space ("Thu Jan  1"). Microsoft's documentation example
 *        shows a zero ("Jan 02"). Expectation here: pre-UCRT CRTs (<140) zero-pad, ucrtbase space-pads. Only the day field is probed.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

static volatile __time64_t vt;
static struct tm tm1 = { 0, 0, 0, 1, 0, 70, 4, 0, 0 };

#if TC_AT_LEAST(140)
#define DAY1 "Thu Jan  1 00:00:00 1970\n"
#define WDAY1 L"Thu Jan  1 00:00:00 1970\n"
#else
#define DAY1 "Thu Jan 01 00:00:00 1970\n"
#define WDAY1 L"Thu Jan 01 00:00:00 1970\n"
#endif

TC_CASE(ctime64_day1)
{
    __time64_t t; char *r; _putenv("TZ=UTC0"); _tzset();
    vt = 0; t = vt; r = _ctime64(&t);
    CHECK(r != NULL); if (r) CHECK_STR(r, DAY1);
}
TC_CASE(wasctime_day1)
{
    struct tm t = tm1; wchar_t *r = _wasctime(&t);
    CHECK(r != NULL); if (r) CHECK_WSTR(r, WDAY1);
}
int main(void) { tc_begin(); TC_RUN(ctime64_day1); TC_RUN(wasctime_day1); return tc_end(); }
