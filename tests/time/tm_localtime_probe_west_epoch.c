/* TC
 * id:    tm_localtime_probe_west_epoch
 * area:  time
 * kind:  positive
 * funcs: _localtime64
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _localtime64: dates before 1970-01-01 UTC midnight fail
 * note:  PROBES behaviour when UTC t is at/after the epoch but local time would be before 1970 (TZ=PST8, t=0). Expected most likely NULL; test accepts NULL or Dec 31 1969 16:00. t=86400 and 3*86400 must be valid.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

static volatile __time64_t v_t;
static void settz(const char *s) { _putenv(s); _tzset(); }

TC_CASE(t0) { struct tm *p; settz("TZ=PST8"); v_t = 0; p = _localtime64((const __time64_t *)&v_t);
    if (p) { CHECK_EQ_I(p->tm_year, 69); CHECK_EQ_I(p->tm_mon, 11); CHECK_EQ_I(p->tm_mday, 31); CHECK_EQ_I(p->tm_hour, 16); } }
TC_CASE(t0_is_null) { struct tm *p; settz("TZ=PST8"); v_t = 0; p = _localtime64((const __time64_t *)&v_t); CHECK(p == NULL); }
TC_CASE(one_day) { struct tm *p; settz("TZ=PST8"); v_t = 86400; p = _localtime64((const __time64_t *)&v_t); CHECK(p != NULL);
    if (p) { CHECK_EQ_I(p->tm_year, 70); CHECK_EQ_I(p->tm_mday, 1); CHECK_EQ_I(p->tm_hour, 16); } }
TC_CASE(three_days) { struct tm *p; settz("TZ=PST8"); v_t = 3 * 86400; p = _localtime64((const __time64_t *)&v_t); CHECK(p != NULL);
    if (p) { CHECK_EQ_I(p->tm_mday, 3); CHECK_EQ_I(p->tm_hour, 16); } }

int main(void)
{
    tc_begin();
    if (TC_AT_LEAST(80)) tc_iph_install(); /* a failing conversion must not abort via the default handler */
    TC_RUN(t0); TC_RUN(t0_is_null); TC_RUN(one_day); TC_RUN(three_days);
    settz("TZ=");
    return tc_end();
}
