/* TC
 * id:    tm_wctime32_basic
 * area:  time
 * funcs: _wctime32
 * crt:   >=80
 * tier:  p2
 * conf:  documented
 * note:  Single-digit days are avoided here because day padding differs between CRT generations; see tm_ctime_probe_day_padding.
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
    vt = 0x7fffffff; t = vt; r = _wctime32(&t);
    CHECK(r != NULL); if (r) CHECK_WSTR(r, L"Tue Jan 19 03:14:07 2038\n");
}
int main(void) { tc_begin(); TC_RUN(epoch); TC_RUN(int_max); return tc_end(); }
