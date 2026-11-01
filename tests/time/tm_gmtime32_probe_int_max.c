/* TC
 * id:    tm_gmtime32_probe_int_max
 * area:  time
 * funcs: _gmtime32 _localtime32
 * crt:   >=90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _gmtime32 (range text ends 23:59:59 Jan 18 2038) vs __time32_t max 03:14:07 Jan 19 2038
 * note:  Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). _gmtime32 accepts INT_MAX (2038-01-19 03:14:07); _localtime32 rejects INT_MAX (NULL) and accepts up to 2147471999 (2038-01-18 23:59:59), matching the docs range text; same for TZ=PST8PDT/JST-9. TZ=UTC0.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <limits.h>
static volatile __time32_t v32;
static void chk(const struct tm *p)
{
    CHECK(p != NULL); if (!p) return;
    CHECK_EQ_I(p->tm_year, 138); CHECK_EQ_I(p->tm_mon, 0); CHECK_EQ_I(p->tm_mday, 19); CHECK_EQ_I(p->tm_hour, 3);
    CHECK_EQ_I(p->tm_min, 14); CHECK_EQ_I(p->tm_sec, 7); CHECK_EQ_I(p->tm_wday, 2); CHECK_EQ_I(p->tm_yday, 18);
}
TC_CASE(gm) { __time32_t t; _putenv("TZ=UTC0"); _tzset(); v32 = INT_MAX; t = v32; chk(_gmtime32(&t)); }
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
TC_CASE(local_int_max_rejected) { __time32_t t; _putenv("TZ=UTC0"); _tzset(); v32 = INT_MAX; t = v32; CHECK(_localtime32(&t) == NULL); }
TC_CASE(local_last_valid)
{
    __time32_t t; struct tm *p; _putenv("TZ=UTC0"); _tzset(); v32 = 2147471999; t = v32; p = _localtime32(&t);
    CHECK(p != NULL); if (!p) return;
    CHECK_EQ_I(p->tm_year, 138); CHECK_EQ_I(p->tm_mon, 0); CHECK_EQ_I(p->tm_mday, 18); CHECK_EQ_I(p->tm_hour, 23); CHECK_EQ_I(p->tm_min, 59); CHECK_EQ_I(p->tm_sec, 59);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(gm); TC_RUN(local_int_max_rejected); TC_RUN(local_last_valid); return tc_end(); }
#else
TC_CASE(local) { __time32_t t; _putenv("TZ=UTC0"); _tzset(); v32 = INT_MAX; t = v32; chk(_localtime32(&t)); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(gm); TC_RUN(local); return tc_end(); }
#endif
