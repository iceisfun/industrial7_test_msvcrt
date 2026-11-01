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
 *        Win10 19045: timezone follows TZ; dstflag follows the OS zone (asserted equal to GetTimeZoneInformation()==TIME_ZONE_ID_DAYLIGHT), not TZ.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <sys/timeb.h>

static void set_tz(const char *s) { _putenv(s); _tzset(); }
/* dstflag: on Windows 10 ucrtbase it came back 1 for TZ=UTC0, JST-9 and PST8PDT alike while the guest OS zone was in daylight time, i.e. it does not
 * follow the TZ variable but the OS zone (GetTimeZoneInformation) and the date. So on ucrtbase it is asserted against the OS daylight state read at run time
 * (deterministic for any zone/date); older CRTs keep the documented expectation 0. */
static int os_dst(void) { TIME_ZONE_INFORMATION z; return GetTimeZoneInformation(&z) == TIME_ZONE_ID_DAYLIGHT; }
#if TC_CRT_VER >= 140
#define CHECK_DST(tb) CHECK_EQ_I((tb).dstflag, os_dst())
#else
#define CHECK_DST(tb) CHECK_EQ_I((tb).dstflag, 0)
#endif

TC_CASE(tz_pst) { struct __timeb64 tb; set_tz("TZ=PST8PDT"); _ftime64(&tb); CHECK_EQ_I(tb.timezone, 480); }
TC_CASE(tz_utc) { struct __timeb64 tb; set_tz("TZ=UTC0"); _ftime64(&tb); CHECK_EQ_I(tb.timezone, 0); CHECK_DST(tb); }
TC_CASE(tz_jst) { struct __timeb64 tb; set_tz("TZ=JST-9"); _ftime64(&tb); CHECK_EQ_I(tb.timezone, -540); CHECK_DST(tb); }
int main(void) { tc_begin(); TC_RUN(tz_pst); TC_RUN(tz_utc); TC_RUN(tz_jst); return tc_end(); }
