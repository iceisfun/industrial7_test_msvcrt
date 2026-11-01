/* TC
 * id:    tm_neg_probe_mktime_errno_range
 * area:  time
 * kind:  negative
 * funcs: _mktime64
 * procs: _mkgmtime64 _mktime32
 * crt:   >=90
 * tier:  p2
 * conf:  observed
 * ref:   MS docs mktime, _mkgmtime (return -1 on failure; errno on range failure is not documented)
 * note:  Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). errno EINVAL on out-of-range results: _mktime64 year 3002 (3001-01-01 is accepted; limit 32536799999), _mkgmtime64 1969-12-31 00:00:00 (mkgmtime64 accepts down to -43200 = 1969-12-31 12:00:00, and 23:59:59 returns the legitimate value -1 with errno untouched), _mktime32 2038-01-19 03:14:08 (32-bit limit is 2147471999). TZ=UTC0.
 *        2038-01-19 03:14:08 (one second past the 32-bit limit); all expected EINVAL. TZ=UTC0.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(__time64_t, _mkgmtime64, (struct tm *));
TC_DECL_PROC(__time32_t, _mktime32, (struct tm *));
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s)
{ t->tm_year = y - 1900; t->tm_mon = mo - 1; t->tm_mday = d; t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_wday = 0; t->tm_yday = 0; t->tm_isdst = 0; }

#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
TC_CASE(mktime64_3002) { struct tm t; mk(&t, 3002, 1, 1, 0, 0, 0); errno = 0; CHECK_EQ_I(_mktime64(&t), -1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(mkgmtime64_1969) { struct tm t; if (!tc_p__mkgmtime64) TC_SKIP("no export"); mk(&t, 1969, 12, 31, 0, 0, 0); errno = 0; CHECK_EQ_I(tc_p__mkgmtime64(&t), -1); CHECK_EQ_I(errno, EINVAL); }
#else
TC_CASE(mktime64_3001) { struct tm t; mk(&t, 3001, 1, 1, 0, 0, 0); errno = 0; CHECK_EQ_I(_mktime64(&t), -1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(mkgmtime64_1969) { struct tm t; if (!tc_p__mkgmtime64) TC_SKIP("no export"); mk(&t, 1969, 12, 31, 23, 59, 59); errno = 0; CHECK_EQ_I(tc_p__mkgmtime64(&t), -1); CHECK_EQ_I(errno, EINVAL); }
#endif
TC_CASE(mktime32_2038) { struct tm t; if (!tc_p__mktime32) TC_SKIP("no export"); mk(&t, 2038, 1, 19, 3, 14, 8); errno = 0; CHECK_EQ_I(tc_p__mktime32(&t), -1); CHECK_EQ_I(errno, EINVAL); }

int main(void)
{
    tc_begin(); tc_iph_install(); TC_LOAD_PROC(_mkgmtime64); TC_LOAD_PROC(_mktime32);
    _putenv("TZ=UTC0"); _tzset();
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
    TC_RUN(mktime64_3002); TC_RUN(mkgmtime64_1969); TC_RUN(mktime32_2038);
#else
    TC_RUN(mktime64_3001); TC_RUN(mkgmtime64_1969); TC_RUN(mktime32_2038);
#endif
    _putenv("TZ=");
    return tc_end();
}
