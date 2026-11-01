/* TC
 * id:    tm_neg_mktime_probe_sec60_rollover
 * area:  time
 * funcs: _mktime64
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _mktime64 (valid through 23:59:59 Dec 31 3000 UTC)
 * note:  Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). _mktime64 normalises 3000-12-31 23:59:60 to 3001-01-01 00:00:00 (32535216000) and ACCEPTS it: the limit is 32536799999 (3001-01-19 07:59:59 UTC), not Dec 31 3000 as docs state. Year 3002 returns -1. TZ=UTC0.
 *        is rejected with -1 (range check after normalisation, as the docs imply) or accepted (check on tm_year only). TZ=UTC0.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

TC_CASE(sec60_rolls_to_3001)
{
    struct tm t; memset(&t, 0, sizeof t);
    _putenv("TZ=UTC0"); _tzset();
    t.tm_year = 1100; t.tm_mon = 11; t.tm_mday = 31; t.tm_hour = 23; t.tm_min = 59; t.tm_sec = 60;
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
    CHECK_EQ_I(_mktime64(&t), 32535216000LL);   /* accepted: limit is 32536799999 */
    CHECK_EQ_I(t.tm_year, 1101); CHECK_EQ_I(t.tm_mon, 0); CHECK_EQ_I(t.tm_mday, 1);
}
TC_CASE(year_3002_rejected)
{
    struct tm t; memset(&t, 0, sizeof t);
    _putenv("TZ=UTC0"); _tzset();
    t.tm_year = 1102; t.tm_mday = 1;
#endif
    CHECK_EQ_I(_mktime64(&t), -1);
}
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
int main(void) { tc_begin(); TC_RUN(sec60_rolls_to_3001); TC_RUN(year_3002_rejected); return tc_end(); }
#else
int main(void) { tc_begin(); TC_RUN(sec60_rolls_to_3001); return tc_end(); }
#endif
