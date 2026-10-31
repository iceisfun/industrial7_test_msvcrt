/* TC
 * id:    tm_localtime_probe_dst_rules
 * area:  time
 * kind:  positive
 * funcs: _localtime64
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs tzset; US DST rules 2007+ (2nd Sun Mar - 1st Sun Nov)
 * note:  PROBES whether ucrtbase applies the post-2007 US DST schedule when DST comes only from the TZ string. Expected most likely: Mar 15 2020 and Oct 28 2020 are DST (isdst 1).
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

static volatile __time64_t v_t;
static void settz(const char *s) { _putenv(s); _tzset(); }

TC_CASE(mar15_is_dst) { struct tm *p; settz("TZ=PST8PDT"); v_t = 1584273600LL; /* 2020-03-15 12:00Z */
    p = _localtime64((const __time64_t *)&v_t); CHECK(p != NULL); if (p) CHECK(p->tm_isdst > 0); }
TC_CASE(oct28_is_dst) { struct tm *p; settz("TZ=PST8PDT"); v_t = 1603886400LL; /* 2020-10-28 12:00Z */
    p = _localtime64((const __time64_t *)&v_t); CHECK(p != NULL); if (p) CHECK(p->tm_isdst > 0); }
TC_CASE(nov1_is_std) { struct tm *p; settz("TZ=PST8PDT"); v_t = 1604232000LL; /* 2020-11-01 12:00Z */
    p = _localtime64((const __time64_t *)&v_t); CHECK(p != NULL); if (p) CHECK_EQ_I(p->tm_isdst, 0); }

int main(void)
{
    tc_begin();
    TC_RUN(mar15_is_dst); TC_RUN(oct28_is_dst); TC_RUN(nov1_is_std);
    settz("TZ=");
    return tc_end();
}
