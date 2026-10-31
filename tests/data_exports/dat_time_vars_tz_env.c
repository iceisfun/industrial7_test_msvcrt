/* TC
 * id:    dat_time_vars_tz_env
 * area:  data_exports
 * funcs: _tzset _putenv _get_daylight _get_timezone _get_tzname
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * note:  _daylight is documented only as nonzero when a DST zone is given, so it is checked as != 0.
 *        The NULL-buffer size query is in dat_time_vars_tzname_sizequery_probe.
 * ref:   MS docs _tzset, _get_timezone, _get_daylight, _get_tzname (TZ=tzn[+|-]hh[:mm[:ss]][dzn]; timezone = seconds west of UTC)
 */
#include "tc.h"
#include <stdlib.h>
#include <time.h>

static void settz(const char *s) { _putenv(s); _tzset(); }

TC_CASE(timezone_28800) { long z = -1; settz("TZ=PST8PDT"); CHECK_EQ_I(_get_timezone(&z), 0); CHECK_EQ_I(z, 28800); settz("TZ="); }
TC_CASE(daylight_1) { int d = -1; settz("TZ=PST8PDT"); CHECK_EQ_I(_get_daylight(&d), 0); CHECK(d != 0); settz("TZ="); }
TC_CASE(tzname0_PST)
{ char b[16] = "xxxx"; size_t n = 0; settz("TZ=PST8PDT");
  CHECK_EQ_I(_get_tzname(&n, b, sizeof b, 0), 0); CHECK_STR(b, "PST"); settz("TZ="); }
TC_CASE(tzname1_PDT)
{ char b[16] = "xxxx"; size_t n = 0; settz("TZ=PST8PDT");
  CHECK_EQ_I(_get_tzname(&n, b, sizeof b, 1), 0); CHECK_STR(b, "PDT"); settz("TZ="); }
TC_CASE(tzname_size_includes_nul)
{ char b[16] = "xxxx"; size_t n = 0, m = 0; settz("TZ=PST8PDT"); b[0] = 0;
  CHECK_EQ_I(_get_tzname(&n, b, sizeof b, 0), 0); CHECK_EQ_U(n, 4);
  (void)m;
  settz("TZ="); }
TC_CASE(utc0_timezone_0_daylight_0)
{ long z = -1; int d = -1; settz("TZ=UTC0"); CHECK_EQ_I(_get_timezone(&z), 0); CHECK_EQ_I(z, 0); CHECK_EQ_I(_get_daylight(&d), 0); CHECK_EQ_I(d, 0); settz("TZ="); }

int main(void)
{
    tc_begin();
    TC_RUN(timezone_28800); TC_RUN(daylight_1); TC_RUN(tzname0_PST); TC_RUN(tzname1_PDT); TC_RUN(tzname_size_includes_nul); TC_RUN(utc0_timezone_0_daylight_0);
    return tc_end();
}
