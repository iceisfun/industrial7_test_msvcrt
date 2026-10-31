/* TC
 * id:    tm_neg_probe_mktime_errno_range
 * area:  time
 * kind:  negative
 * funcs: _mktime64
 * procs: _mkgmtime64 _mktime32
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs mktime, _mkgmtime (return -1 on failure; errno on range failure is not documented)
 * note:  Probes the errno set by out-of-range results: _mktime64 year 3001, _mkgmtime64 year 1969, _mktime32 on
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

TC_CASE(mktime64_3001) { struct tm t; mk(&t, 3001, 1, 1, 0, 0, 0); errno = 0; CHECK_EQ_I(_mktime64(&t), -1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(mkgmtime64_1969) { struct tm t; if (!tc_p__mkgmtime64) TC_SKIP("no export"); mk(&t, 1969, 12, 31, 23, 59, 59); errno = 0; CHECK_EQ_I(tc_p__mkgmtime64(&t), -1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(mktime32_2038) { struct tm t; if (!tc_p__mktime32) TC_SKIP("no export"); mk(&t, 2038, 1, 19, 3, 14, 8); errno = 0; CHECK_EQ_I(tc_p__mktime32(&t), -1); CHECK_EQ_I(errno, EINVAL); }

int main(void)
{
    tc_begin(); tc_iph_install(); TC_LOAD_PROC(_mkgmtime64); TC_LOAD_PROC(_mktime32);
    _putenv("TZ=UTC0"); _tzset();
    TC_RUN(mktime64_3001); TC_RUN(mkgmtime64_1969); TC_RUN(mktime32_2038);
    _putenv("TZ=");
    return tc_end();
}
