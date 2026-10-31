/* TC
 * id:    tm_localtime64_isdst_flag
 * area:  time
 * kind:  positive
 * funcs: _localtime64
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _localtime64; tm_isdst > 0 if DST in effect, 0 if not
 * note:  TZ string selects zone; DST only when the TZ string names a DST zone
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

static volatile __time64_t v_t;
static void settz(const char *s) { _putenv(s); _tzset(); }

TC_CASE(utc) { struct tm *p; settz("TZ=UTC0"); v_t = 1593864000LL; p = _localtime64((const __time64_t *)&v_t); CHECK(p != NULL); if (p) CHECK_EQ_I(p->tm_isdst, 0); }
TC_CASE(pst_winter) { struct tm *p; settz("TZ=PST8PDT"); v_t = 1577880000LL; /* 2020-01-01 12:00Z */
    p = _localtime64((const __time64_t *)&v_t); CHECK(p != NULL); if (p) { CHECK_EQ_I(p->tm_isdst, 0); CHECK_EQ_I(p->tm_hour, 4); } }
TC_CASE(pst_summer) { struct tm *p; settz("TZ=PST8PDT"); v_t = 1593864000LL; /* 2020-07-04 12:00Z */
    p = _localtime64((const __time64_t *)&v_t); CHECK(p != NULL); if (p) { CHECK(p->tm_isdst > 0); CHECK_EQ_I(p->tm_hour, 5); } }
TC_CASE(est_no_dst_rule) { struct tm *p; settz("TZ=EST5"); v_t = 1593864000LL;
    p = _localtime64((const __time64_t *)&v_t); CHECK(p != NULL); if (p) { CHECK_EQ_I(p->tm_isdst, 0); CHECK_EQ_I(p->tm_hour, 7); } }

int main(void)
{
    tc_begin();
    TC_RUN(utc); TC_RUN(pst_winter); TC_RUN(pst_summer); TC_RUN(est_no_dst_rule);
    settz("TZ=");
    return tc_end();
}
