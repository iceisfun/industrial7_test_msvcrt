/* TC
 * id:    dat_time_data_win7
 * area:  data_exports
 * funcs: _tzset _putenv
 * procs: _timezone _daylight _tzname _dstbias
 * crt:   msvcr120 msvcr110 msvcr100 msvcr80 msvcrt
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _tzset, _timezone/_daylight/_tzname (Global variables set from TZ)
 * note:  _daylight is documented as "nonzero if a daylight-saving zone is specified in TZ", so only != 0 is asserted.
 * note:  The data exports are resolved with GetProcAddress (address of the exported variable) so the test does not depend on
 *        the mingw header choosing accessor functions. TZ=PST8PDT: 8 hours west of UTC (28800 s), daylight saving zone present.
 */
#include "tc.h"
#include <stdlib.h>
#include <time.h>

static long *p_timezone; static int *p_daylight; static char **p_tzname;

TC_CASE(exported) { CHECK(p_timezone != NULL); CHECK(p_daylight != NULL); CHECK(p_tzname != NULL); }
TC_CASE(timezone_28800) { if (!p_timezone) TC_SKIP("no _timezone"); CHECK_EQ_I(*p_timezone, 28800); }
TC_CASE(daylight_nonzero) { if (!p_daylight) TC_SKIP("no _daylight"); CHECK(*p_daylight != 0); }
TC_CASE(tzname_pst_pdt)
{
    if (!p_tzname) TC_SKIP("no _tzname");
    CHECK_STR(p_tzname[0], "PST"); CHECK_STR(p_tzname[1], "PDT");
}
TC_CASE(accessors_equal_data)
{
    /* the data exports are the same objects the accessor-using headers read; compare to a second _tzset result */
    long t1; int d1;
    if (!p_timezone || !p_daylight) TC_SKIP("no data");
    t1 = *p_timezone; d1 = *p_daylight;
    _tzset();
    CHECK_EQ_I(*p_timezone, t1); CHECK_EQ_I(*p_daylight, d1);
}

int main(void)
{
    tc_begin();
    p_timezone = (long *)tc_crt_proc("_timezone");
    p_daylight = (int *)tc_crt_proc("_daylight");
    p_tzname = (char **)tc_crt_proc("_tzname");
    _putenv("TZ=PST8PDT");
    _tzset();
    TC_RUN(exported); TC_RUN(timezone_28800); TC_RUN(daylight_nonzero); TC_RUN(tzname_pst_pdt); TC_RUN(accessors_equal_data);
    return tc_end();
}
