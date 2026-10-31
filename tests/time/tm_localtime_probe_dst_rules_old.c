/* TC
 * id:    tm_localtime_probe_dst_rules_old
 * area:  time
 * kind:  positive
 * funcs: _localtime64
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   MS docs tzset; pre-2007 US rules (1st Sun Apr - last Sun Oct)
 * note:  PROBES whether legacy CRTs apply the pre-2007 US DST schedule when DST comes from the TZ string. Expected most likely: Mar 15 2020 and Oct 28 2020 are NOT DST (isdst 0).
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

static volatile __time64_t v_t;
static void settz(const char *s) { _putenv(s); _tzset(); }

TC_CASE(mar15_not_dst) { struct tm *p; settz("TZ=PST8PDT"); v_t = 1584273600LL; /* 2020-03-15 12:00Z */
    p = _localtime64((const __time64_t *)&v_t); CHECK(p != NULL); if (p) CHECK_EQ_I(p->tm_isdst, 0); }
TC_CASE(oct28_not_dst) { struct tm *p; settz("TZ=PST8PDT"); v_t = 1603886400LL; /* 2020-10-28 12:00Z */
    p = _localtime64((const __time64_t *)&v_t); CHECK(p != NULL); if (p) CHECK_EQ_I(p->tm_isdst, 0); }

int main(void)
{
    tc_begin();
    TC_RUN(mar15_not_dst); TC_RUN(oct28_not_dst);
    settz("TZ=");
    return tc_end();
}
