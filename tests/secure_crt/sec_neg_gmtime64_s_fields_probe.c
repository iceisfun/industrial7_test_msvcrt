/* TC
 * id:    sec_neg_gmtime64_s_fields_probe
 * area:  secure_crt
 * kind:  negative
 * funcs:
 * procs: _gmtime64_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _gmtime64_s
 * note:  Observed on Win10 22H2 19045 ucrtbase: _gmtime64_s(-1) SUCCEEDS (1969-12-31 23:59:59; accepted range starts at
 *        -43200), so the old failure input -1 was wrong. A real range failure (time -86400, or 30 days past 32535215999)
 *        returns EINVAL (22), leaves the handler uncalled and sets all nine tm fields to -1, which is what the test probes. The
 *        ~12 h / ~18 day slack at the ends is an implementation detail (observed, not documented); inputs here are chosen far
 *        outside it. DEVIATES from the MS docs _gmtime64_s error table (sourceTime < 0 -> EINVAL, all fields -1): probe lines
 *        gm_-1=0, gm_-43200=0, gm_-86400=22 show negative times down to -43200 are accepted.
 */
#include "tc.h"
#include <time.h>

typedef int (__cdecl *gm64_t)(struct tm *, const __int64 *);
static gm64_t p_gm64;

static void check_fail(__int64 t0)
{
    struct tm x; volatile __int64 t = t0; int r;
    x.tm_sec = x.tm_min = x.tm_hour = x.tm_mday = x.tm_mon = x.tm_year = x.tm_wday = x.tm_yday = x.tm_isdst = 7;
    r = p_gm64(&x, (const __int64 *)&t);
    CHECK_EQ_I(r, 22);
    CHECK_EQ_I(x.tm_sec, -1); CHECK_EQ_I(x.tm_min, -1); CHECK_EQ_I(x.tm_hour, -1); CHECK_EQ_I(x.tm_mday, -1); CHECK_EQ_I(x.tm_mon, -1);
    CHECK_EQ_I(x.tm_year, -1); CHECK_EQ_I(x.tm_wday, -1); CHECK_EQ_I(x.tm_yday, -1); CHECK_EQ_I(x.tm_isdst, -1);
}
/* observed: -1 and -43200 are VALID inputs (12 h of slack below the epoch); the first clearly invalid negative time is -86400 */
TC_CASE(fields_minus_one_after_failure) { check_fail(-86400); }
/* observed: the upper limit has ~18 days of slack above 23:59:59 3000-12-31 UTC (32535215999); +30 days is clearly invalid */
TC_CASE(fields_minus_one_after_failure_beyond_max) { check_fail(32535215999LL + 30LL * 86400LL); }
TC_CASE(minus_one_is_valid)
{
    struct tm x; volatile __int64 t = -1; int r = p_gm64(&x, (const __int64 *)&t);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(x.tm_year, 69); CHECK_EQ_I(x.tm_mon, 11); CHECK_EQ_I(x.tm_mday, 31);
    CHECK_EQ_I(x.tm_hour, 23); CHECK_EQ_I(x.tm_min, 59); CHECK_EQ_I(x.tm_sec, 59);
}
int main(void)
{
    tc_begin();
    p_gm64 = (gm64_t)tc_crt_proc("_gmtime64_s");
    CHECK(p_gm64); if (!p_gm64) return tc_end();
    tc_iph_install();
    TC_RUN(fields_minus_one_after_failure); TC_RUN(fields_minus_one_after_failure_beyond_max); TC_RUN(minus_one_is_valid);
    return tc_end();
}
