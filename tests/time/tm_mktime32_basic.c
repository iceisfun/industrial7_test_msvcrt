/* TC
 * id:    tm_mktime32_basic
 * area:  time
 * funcs: _mktime32
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _mktime32 (range 1970 to 23:59:59 Jan 18 2038 UTC)
 * note:  Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). _mktime32 last valid time is 2038-01-18 23:59:59 UTC = 2147471999 (docs range text); 2038-01-19 03:14:07 returns -1. msvcrt.dll lacks _mktime32. TZ=UTC0
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

static void settz(const char *s) { _putenv(s); _tzset(); }
static struct tm mk(int y, int mo, int d, int h, int mi, int s, int dst)
{
    struct tm t; memset(&t, 0, sizeof t);
    t.tm_year = y - 1900; t.tm_mon = mo - 1; t.tm_mday = d; t.tm_hour = h; t.tm_min = mi; t.tm_sec = s; t.tm_isdst = dst;
    return t;
}

TC_CASE(leap) { struct tm t = mk(2000, 2, 29, 0, 0, 0, 0); settz("TZ=UTC0"); CHECK_EQ_I(_mktime32(&t), 951782400); CHECK_EQ_I(t.tm_wday, 2); }
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
TC_CASE(max32) { struct tm t = mk(2038, 1, 18, 23, 59, 59, 0); settz("TZ=UTC0"); CHECK_EQ_I(_mktime32(&t), 2147471999); }
#else
TC_CASE(max32) { struct tm t = mk(2038, 1, 19, 3, 14, 7, 0); settz("TZ=UTC0"); CHECK_EQ_I(_mktime32(&t), 2147483647); }
#endif
TC_CASE(epoch) { struct tm t = mk(1970, 1, 1, 0, 0, 0, 0); settz("TZ=UTC0"); CHECK_EQ_I(_mktime32(&t), 0); }
int main(void) { tc_begin(); TC_RUN(leap); TC_RUN(max32); TC_RUN(epoch); return tc_end(); }
