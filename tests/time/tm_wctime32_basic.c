/* TC
 * id:    tm_wctime32_basic
 * area:  time
 * funcs: _wctime32
 * crt:   >=80
 * tier:  p2
 * conf:  documented
 * note:  Single-digit days avoided (day padding differs between CRT generations). Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). _wctime32 accepts up to 2038-01-18 23:59:59 UTC (docs range text) and returns NULL for INT_MAX.
 * ref:   MS docs ctime, _wctime32
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

static volatile __time32_t vt;

TC_CASE(epoch)
{
    __time32_t t; wchar_t *r; _putenv("TZ=UTC0"); _tzset();
    vt = 864000; t = vt; r = _wctime32(&t);
    CHECK(r != NULL); if (r) CHECK_WSTR(r, L"Sun Jan 11 00:00:00 1970\n");
}
TC_CASE(int_max)
{
    __time32_t t; wchar_t *r; _putenv("TZ=UTC0"); _tzset();
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
    vt = 2147471999; t = vt; r = _wctime32(&t);
    CHECK(r != NULL); if (r) CHECK_WSTR(r, L"Mon Jan 18 23:59:59 2038\n");
#endif
    vt = 0x7fffffff; t = vt; r = _wctime32(&t);
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
    CHECK(r == NULL);   /* INT_MAX is past the _wctime32 limit */
#else
    CHECK(r != NULL); if (r) CHECK_WSTR(r, L"Tue Jan 19 03:14:07 2038\n");
#endif
}
int main(void) { tc_begin(); TC_RUN(epoch); TC_RUN(int_max); return tc_end(); }
