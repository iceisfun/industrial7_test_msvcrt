/* TC
 * id:    tm_ctime32_basic
 * area:  time
 * funcs: _ctime32
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * note:  Single-digit days are avoided here because day padding differs between CRT generations; see tm_ctime_probe_day_padding.
 * ref:   MS docs ctime, _ctime32
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

static volatile __time32_t vt;

TC_CASE(epoch)
{
    __time32_t t; char *r; _putenv("TZ=UTC0"); _tzset();
    vt = 864000; t = vt; r = _ctime32(&t);
    CHECK(r != NULL); if (r) CHECK_STR(r, "Sun Jan 11 00:00:00 1970\n");
}
TC_CASE(int_max)
{
    __time32_t t; char *r; _putenv("TZ=UTC0"); _tzset();
    vt = 0x7fffffff; t = vt; r = _ctime32(&t);
    CHECK(r != NULL); if (r) CHECK_STR(r, "Tue Jan 19 03:14:07 2038\n");
}
TC_CASE(billion)
{
    __time32_t t; char *r; _putenv("TZ=UTC0"); _tzset();
    vt = 1000259200; t = vt; r = _ctime32(&t);
    CHECK(r != NULL); if (r) CHECK_STR(r, "Wed Sep 12 01:46:40 2001\n");
}
int main(void) { tc_begin(); TC_RUN(epoch); TC_RUN(int_max); TC_RUN(billion); return tc_end(); }
