/* TC
 * id:    tm_neg_mktime32_overflow
 * area:  time
 * kind:  negative
 * funcs: _mktime32
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs mktime: _mktime32 range ends 23:59:59 Jan 18 2038 UTC
 * note:  Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). _mktime32 last valid value is 2038-01-18 23:59:59 UTC (2147471999), per docs range text; 2038-01-19 00:00:00 (and 03:14:07) return -1.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static void settz(const char *s) { _putenv(s); _tzset(); }
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s, int dst)
{
    volatile int v = y; t->tm_year = v - 1900; t->tm_mon = mo - 1; t->tm_mday = d;
    t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_isdst = dst; t->tm_wday = 0; t->tm_yday = 0;
}

#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
TC_CASE(last_valid) { struct tm t; settz("TZ=UTC0"); mk(&t, 2038, 1, 18, 23, 59, 59, 0); CHECK_EQ_I(_mktime32(&t), 2147471999); }
TC_CASE(int_max_time_rejected) { struct tm t; settz("TZ=UTC0"); mk(&t, 2038, 1, 19, 3, 14, 7, 0); CHECK_EQ_I(_mktime32(&t), -1); }
TC_CASE(one_past) { struct tm t; settz("TZ=UTC0"); mk(&t, 2038, 1, 19, 0, 0, 0, 0); CHECK_EQ_I(_mktime32(&t), -1); }
#else
TC_CASE(last_valid) { struct tm t; settz("TZ=UTC0"); mk(&t, 2038, 1, 19, 3, 14, 7, 0); CHECK_EQ_I(_mktime32(&t), 2147483647); }
TC_CASE(one_past) { struct tm t; settz("TZ=UTC0"); mk(&t, 2038, 1, 19, 3, 14, 8, 0); CHECK_EQ_I(_mktime32(&t), -1); }
#endif
TC_CASE(year_2100) { struct tm t; settz("TZ=UTC0"); mk(&t, 2100, 1, 1, 0, 0, 0, 0); CHECK_EQ_I(_mktime32(&t), -1); }
TC_CASE(before_1970) { struct tm t; settz("TZ=UTC0"); mk(&t, 1969, 12, 31, 23, 59, 59, 0); CHECK_EQ_I(_mktime32(&t), -1); }
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
int main(void) { tc_begin(); TC_RUN(last_valid); TC_RUN(int_max_time_rejected); TC_RUN(one_past); TC_RUN(year_2100); TC_RUN(before_1970); return tc_end(); }
#else
int main(void) { tc_begin(); TC_RUN(last_valid); TC_RUN(one_past); TC_RUN(year_2100); TC_RUN(before_1970); return tc_end(); }
#endif
