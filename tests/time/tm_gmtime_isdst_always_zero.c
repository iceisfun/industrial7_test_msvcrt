/* TC
 * id:    tm_gmtime_isdst_always_zero
 * area:  time
 * funcs: _gmtime64 _putenv _tzset
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs gmtime/_gmtime64 (tm_isdst is always 0 for gmtime); C11 7.27.3.3
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>

static int isdst_of(__time64_t v) { volatile __time64_t t = v; struct tm *p = _gmtime64((__time64_t *)&t); return p ? p->tm_isdst : -99; }
TC_CASE(pacific_summer) { _putenv("TZ=PST8PDT"); _tzset(); CHECK_EQ_I(isdst_of(1593864000LL), 0); _putenv("TZ="); _tzset(); } /* 2020-07-04 12:00 UTC */
TC_CASE(pacific_winter) { _putenv("TZ=PST8PDT"); _tzset(); CHECK_EQ_I(isdst_of(1579089600LL), 0); _putenv("TZ="); _tzset(); } /* 2020-01-15 12:00 UTC */
TC_CASE(utc) { _putenv("TZ=UTC0"); _tzset(); CHECK_EQ_I(isdst_of(1593864000LL), 0); _putenv("TZ="); _tzset(); }
int main(void) { tc_begin(); TC_RUN(pacific_summer); TC_RUN(pacific_winter); TC_RUN(utc); return tc_end(); }
