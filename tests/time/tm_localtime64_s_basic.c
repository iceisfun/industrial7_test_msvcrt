/* TC
 * id:    tm_localtime64_s_basic
 * area:  time
 * kind:  positive
 * funcs: _localtime64_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _localtime_s, _localtime32_s, _localtime64_s
 * note:  returns 0 on success and fills the caller's struct tm
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

static volatile __time64_t v_t;
static void settz(const char *s) { _putenv(s); _tzset(); }

TC_CASE(utc_epoch) { struct tm t; errno_t e; settz("TZ=UTC0"); v_t = 0;
    e = _localtime64_s(&t, (const __time64_t *)&v_t); CHECK_EQ_I(e, 0);
    CHECK_EQ_I(t.tm_year, 70); CHECK_EQ_I(t.tm_mon, 0); CHECK_EQ_I(t.tm_mday, 1);
    CHECK_EQ_I(t.tm_hour, 0); CHECK_EQ_I(t.tm_min, 0); CHECK_EQ_I(t.tm_sec, 0); CHECK_EQ_I(t.tm_wday, 4); CHECK_EQ_I(t.tm_yday, 0); }
TC_CASE(jst_epoch) { struct tm t; errno_t e; settz("TZ=JST-9"); v_t = 0;
    e = _localtime64_s(&t, (const __time64_t *)&v_t); CHECK_EQ_I(e, 0);
    CHECK_EQ_I(t.tm_year, 70); CHECK_EQ_I(t.tm_mday, 1); CHECK_EQ_I(t.tm_hour, 9); CHECK_EQ_I(t.tm_min, 0); CHECK_EQ_I(t.tm_isdst, 0); }
TC_CASE(pdt_july4) { struct tm t; errno_t e; settz("TZ=PST8PDT"); v_t = 1593864000LL; /* 2020-07-04 12:00Z */
    e = _localtime64_s(&t, (const __time64_t *)&v_t); CHECK_EQ_I(e, 0);
    CHECK_EQ_I(t.tm_year, 120); CHECK_EQ_I(t.tm_mon, 6); CHECK_EQ_I(t.tm_mday, 4);
    CHECK_EQ_I(t.tm_hour, 5); CHECK(t.tm_isdst > 0); CHECK_EQ_I(t.tm_wday, 6); CHECK_EQ_I(t.tm_yday, 185); }
TC_CASE(caller_buffer) { struct tm a, b; settz("TZ=UTC0"); v_t = 86400;
    CHECK_EQ_I(_localtime64_s(&a, (const __time64_t *)&v_t), 0);
    v_t = 172800;
    CHECK_EQ_I(_localtime64_s(&b, (const __time64_t *)&v_t), 0);
    CHECK_EQ_I(a.tm_mday, 2); CHECK_EQ_I(b.tm_mday, 3); }

int main(void)
{
    tc_begin();
    TC_RUN(utc_epoch); TC_RUN(jst_epoch); TC_RUN(pdt_july4); TC_RUN(caller_buffer);
    settz("TZ=");
    return tc_end();
}
