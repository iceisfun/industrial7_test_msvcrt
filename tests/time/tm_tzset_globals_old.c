/* TC
 * id:    tm_tzset_globals_old
 * area:  time
 * funcs: _tzset
 * procs: __p__timezone __p__daylight __p__tzname
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120 msvcrt
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _tzset, _timezone/_daylight/_tzname global variables
 * note:  The Win7-era DLLs expose the globals through __p__timezone/__p__daylight/__p__tzname (msvcr80+) or as
 *        plain data exports _timezone/_daylight/_tzname (msvcrt.dll). The test resolves whichever form exists.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

static long  *tz_ptr(void)
{
    FARPROC p = tc_crt_proc("__p__timezone");
    if (p) return ((long *(__cdecl *)(void))p)();
    return (long *)tc_crt_proc("_timezone");
}
static int *dl_ptr(void)
{
    FARPROC p = tc_crt_proc("__p__daylight");
    if (p) return ((int *(__cdecl *)(void))p)();
    return (int *)tc_crt_proc("_daylight");
}
static char **tn_ptr(void)
{
    FARPROC p = tc_crt_proc("__p__tzname");
    if (p) return ((char **(__cdecl *)(void))p)();
    return (char **)tc_crt_proc("_tzname");
}
static void set_tz(const char *s) { _putenv(s); _tzset(); }

TC_CASE(resolved)
{
    CHECK(tz_ptr() != NULL); CHECK(dl_ptr() != NULL); CHECK(tn_ptr() != NULL);
}
TC_CASE(pst8pdt)
{
    long *tz = tz_ptr(); int *dl = dl_ptr(); char **tn = tn_ptr();
    if (!tz || !dl || !tn) TC_SKIP("globals not resolvable");
    set_tz("TZ=PST8PDT");
    CHECK_EQ_I(*tz, 28800);
    CHECK_EQ_I(*dl, 1);
    CHECK_STR(tn[0], "PST");
    CHECK_STR(tn[1], "PDT");
}
TC_CASE(utc0)
{
    long *tz = tz_ptr(); int *dl = dl_ptr(); char **tn = tn_ptr();
    if (!tz || !dl || !tn) TC_SKIP("globals not resolvable");
    set_tz("TZ=UTC0");
    CHECK_EQ_I(*tz, 0);
    CHECK_EQ_I(*dl, 0);
    CHECK_STR(tn[0], "UTC");
}
TC_CASE(east)
{
    long *tz = tz_ptr(); int *dl = dl_ptr();
    if (!tz || !dl) TC_SKIP("globals not resolvable");
    set_tz("TZ=CET-1");
    CHECK_EQ_I(*tz, -3600);
    CHECK_EQ_I(*dl, 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(resolved); TC_RUN(pst8pdt); TC_RUN(utc0); TC_RUN(east);
    set_tz("TZ=");
    return tc_end();
}
