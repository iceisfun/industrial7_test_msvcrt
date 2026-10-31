/* TC
 * id:    tm_ftime_probe_tz
 * area:  time
 * funcs: _ftime64
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _ftime64 (timezone = minutes west of UTC)
 * note:  PROBE: whether _ftime64's timezone/dstflag follow the TZ environment variable (as _timezone/60) rather than the
 *        OS time-zone bias, on every CRT generation. Expected: PST8PDT -> 480, UTC0 -> 0 (dstflag 0), JST-9 -> -540.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <sys/timeb.h>

static void set_tz(const char *s) { _putenv(s); _tzset(); }

TC_CASE(tz_pst) { struct __timeb64 tb; set_tz("TZ=PST8PDT"); _ftime64(&tb); CHECK_EQ_I(tb.timezone, 480); }
TC_CASE(tz_utc) { struct __timeb64 tb; set_tz("TZ=UTC0"); _ftime64(&tb); CHECK_EQ_I(tb.timezone, 0); CHECK_EQ_I(tb.dstflag, 0); }
TC_CASE(tz_jst) { struct __timeb64 tb; set_tz("TZ=JST-9"); _ftime64(&tb); CHECK_EQ_I(tb.timezone, -540); CHECK_EQ_I(tb.dstflag, 0); }
int main(void) { tc_begin(); TC_RUN(tz_pst); TC_RUN(tz_utc); TC_RUN(tz_jst); return tc_end(); }
