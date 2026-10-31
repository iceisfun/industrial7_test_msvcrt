/* TC
 * id:    tm_ctime64_basic
 * area:  time
 * funcs: _ctime64
 * crt:   all
 * tier:  p0
 * conf:  documented
 * note:  Single-digit days are avoided here because day padding differs between CRT generations; see tm_ctime_probe_day_padding.
 * ref:   MS docs ctime, _ctime64, _ctime32 (26-char string, TZ environment variable)
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

static volatile __time64_t vt;

TC_CASE(epoch_utc)
{
    __time64_t t; char *r; _putenv("TZ=UTC0"); _tzset();
    vt = 864000; t = vt; r = _ctime64(&t);
    CHECK(r != NULL); if (r) CHECK_STR(r, "Sun Jan 11 00:00:00 1970\n");
}
TC_CASE(leap_day)
{
    __time64_t t; char *r; _putenv("TZ=UTC0"); _tzset();
    vt = 951782400; t = vt; r = _ctime64(&t);
    CHECK(r != NULL); if (r) CHECK_STR(r, "Tue Feb 29 00:00:00 2000\n");
}
TC_CASE(jst_offset)
{
    __time64_t t; char *r; _putenv("TZ=JST-9"); _tzset();
    vt = 86400 * 10; t = vt; r = _ctime64(&t);
    CHECK(r != NULL); if (r) CHECK_STR(r, "Sun Jan 11 09:00:00 1970\n");
    _putenv("TZ=UTC0"); _tzset();
}
TC_CASE(equals_asctime_localtime)
{
    __time64_t t; char a[40]; char *r; struct tm *tm; _putenv("TZ=JST-9"); _tzset();
    vt = 1000259200; t = vt; r = _ctime64(&t);
    CHECK(r != NULL); if (!r) return;
    { int i = 0; for (; r[i] && i < 39; i++) a[i] = r[i]; a[i] = 0; }
    tm = _localtime64(&t); CHECK(tm != NULL); if (!tm) return;
    r = asctime(tm); CHECK(r != NULL); if (r) CHECK_STR(r, a);
    CHECK_STR(a, "Wed Sep 12 10:46:40 2001\n");
    _putenv("TZ=UTC0"); _tzset();
}
int main(void) { tc_begin(); TC_RUN(epoch_utc); TC_RUN(leap_day); TC_RUN(jst_offset); TC_RUN(equals_asctime_localtime); return tc_end(); }
