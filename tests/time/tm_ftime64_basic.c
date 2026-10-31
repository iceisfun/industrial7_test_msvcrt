/* TC
 * id:    tm_ftime64_basic
 * area:  time
 * funcs: _ftime64
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _ftime, _ftime32, _ftime64
 * note:  timezone/dstflag values vs TZ moved to tm_ftime_probe_tz (not documented precisely).
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <sys/timeb.h>

static void set_tz(const char *s) { _putenv(s); _tzset(); }

TC_CASE(near_time64)
{
    struct __timeb64 tb; __time64_t t; tb.time = 0; tb.millitm = 5000;
    set_tz("TZ=UTC0");
    _ftime64(&tb); t = _time64(NULL);
    CHECK(tb.time >= t - 3 && tb.time <= t + 3);
    CHECK(tb.millitm < 1000);
}
TC_CASE(dstflag_range) { struct __timeb64 tb; set_tz("TZ=PST8PDT"); _ftime64(&tb); CHECK(tb.dstflag == 0 || tb.dstflag == 1); }
int main(void) { tc_begin(); TC_RUN(near_time64); TC_RUN(dstflag_range); return tc_end(); }
