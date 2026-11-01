/* TC
 * id:    tm_localtime32_basic
 * area:  time
 * kind:  positive
 * funcs: _localtime32 _localtime32_s
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _localtime32, _localtime32_s
 * note:  Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). _localtime32 range ends 2038-01-18 23:59:59 UTC (2147471999), as the docs say; INT_MAX returns NULL (see tm_gmtime32_probe_int_max).
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

static volatile __time32_t v_t;
static void settz(const char *s) { _putenv(s); _tzset(); }

#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
TC_CASE(utc_max) { struct tm *p; settz("TZ=UTC0"); v_t = 2147471999; p = _localtime32((const __time32_t *)&v_t); CHECK(p != NULL);
    if (p) { CHECK_EQ_I(p->tm_year, 138); CHECK_EQ_I(p->tm_mon, 0); CHECK_EQ_I(p->tm_mday, 18); CHECK_EQ_I(p->tm_hour, 23); CHECK_EQ_I(p->tm_min, 59); CHECK_EQ_I(p->tm_sec, 59); } }
#else
TC_CASE(utc_max) { struct tm *p; settz("TZ=UTC0"); v_t = 0x7fffffff; p = _localtime32((const __time32_t *)&v_t); CHECK(p != NULL);
    if (p) { CHECK_EQ_I(p->tm_year, 138); CHECK_EQ_I(p->tm_mon, 0); CHECK_EQ_I(p->tm_mday, 19); CHECK_EQ_I(p->tm_hour, 3); CHECK_EQ_I(p->tm_min, 14); CHECK_EQ_I(p->tm_sec, 7); } }
#endif
TC_CASE(jst) { struct tm *p; settz("TZ=JST-9"); v_t = 0; p = _localtime32((const __time32_t *)&v_t); CHECK(p != NULL);
    if (p) { CHECK_EQ_I(p->tm_year, 70); CHECK_EQ_I(p->tm_mday, 1); CHECK_EQ_I(p->tm_hour, 9); } }
TC_CASE(s_variant) { struct tm t; settz("TZ=UTC0"); v_t = 3600; CHECK_EQ_I(_localtime32_s(&t, (const __time32_t *)&v_t), 0);
    CHECK_EQ_I(t.tm_year, 70); CHECK_EQ_I(t.tm_mday, 1); CHECK_EQ_I(t.tm_hour, 1); }
TC_CASE(s_variant_jst) { struct tm t; settz("TZ=JST-9"); v_t = 0; CHECK_EQ_I(_localtime32_s(&t, (const __time32_t *)&v_t), 0); CHECK_EQ_I(t.tm_hour, 9); }

int main(void)
{
    tc_begin();
    TC_RUN(utc_max); TC_RUN(jst); TC_RUN(s_variant); TC_RUN(s_variant_jst);
    settz("TZ=");
    return tc_end();
}
