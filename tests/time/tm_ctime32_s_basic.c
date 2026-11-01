/* TC
 * id:    tm_ctime32_s_basic
 * area:  time
 * funcs:
 * procs: _ctime32_s
 * crt:   >=90 msvcrt
 * tier:  p1
 * conf:  documented
 * note:  Single-digit days avoided (day padding differs between CRT generations). Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). _ctime32_s accepts up to 2038-01-18 23:59:59 UTC (2147471999) and rejects INT_MAX (EINVAL), as the MS docs range text says (ends Jan 18 2038); stable across TZ=UTC0/PST8PDT/JST-9.
 * ref:   MS docs ctime_s, _ctime32_s
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

TC_DECL_PROC(int, _ctime32_s, (char *, size_t, const __time32_t *));
static volatile __time32_t vt;

TC_CASE(epoch)
{
    __time32_t t; char b[26]; _putenv("TZ=UTC0"); _tzset();
    if (!tc_p__ctime32_s) TC_SKIP("not exported");
    vt = 864000; t = vt;
    CHECK_EQ_I(tc_p__ctime32_s(b, 26, &t), 0);
    CHECK_STR(b, "Sun Jan 11 00:00:00 1970\n");
}
TC_CASE(int_max)
{
    __time32_t t; char b[26]; _putenv("TZ=UTC0"); _tzset();
    if (!tc_p__ctime32_s) TC_SKIP("not exported");
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
    vt = 2147471999; t = vt;   /* 2038-01-18 23:59:59 UTC: last valid, per docs */
#else
    vt = 0x7fffffff; t = vt;
#endif
    CHECK_EQ_I(tc_p__ctime32_s(b, 26, &t), 0);
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
    CHECK_STR(b, "Mon Jan 18 23:59:59 2038\n");
#else
    CHECK_STR(b, "Tue Jan 19 03:14:07 2038\n");
#endif
}
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
TC_CASE(int_max_rejected)
{
    __time32_t t; char b[26]; _putenv("TZ=UTC0"); _tzset();
    if (!tc_p__ctime32_s) TC_SKIP("not exported");
    vt = 0x7fffffff; t = vt; b[0] = 'x';
    CHECK_EQ_I(tc_p__ctime32_s(b, 26, &t), EINVAL);
    CHECK_EQ_I(b[0], 0);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_ctime32_s); TC_RUN(epoch); TC_RUN(int_max); TC_RUN(int_max_rejected); return tc_end(); }
#else
int main(void) { tc_begin(); TC_LOAD_PROC(_ctime32_s); TC_RUN(epoch); TC_RUN(int_max); return tc_end(); }
#endif
