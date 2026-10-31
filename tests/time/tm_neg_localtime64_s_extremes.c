/* TC
 * id:    tm_neg_localtime64_s_extremes
 * area:  time
 * kind:  negative
 * funcs:
 * procs: _localtime64_s _localtime32_s _gmtime32_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _localtime_s, _localtime64_s, _gmtime_s, _gmtime32_s: EINVAL if the time is before 1970 or beyond the range limit
 * note:  tm contents after failure and whether the invalid-parameter handler runs for range errors are not asserted here
 *        (see tm_neg_localtime64_s_probe_fill). TZ=UTC0.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
TC_DECL_PROC(int, _localtime64_s, (struct tm *, const __time64_t *));
TC_DECL_PROC(int, _localtime32_s, (struct tm *, const __time32_t *));
TC_DECL_PROC(int, _gmtime32_s, (struct tm *, const __time32_t *));
static volatile __time64_t v64;
static volatile __time32_t v32;
static void tz(void) { _putenv("TZ=UTC0"); _tzset(); }
TC_CASE(local64_max) { struct tm tm; __time64_t t; tz(); v64 = LLONG_MAX; t = v64; CHECK_EQ_I(tc_p__localtime64_s(&tm, &t), EINVAL); }
TC_CASE(local64_min) { struct tm tm; __time64_t t; tz(); v64 = LLONG_MIN; t = v64; CHECK_EQ_I(tc_p__localtime64_s(&tm, &t), EINVAL); }
TC_CASE(local32_min) { struct tm tm; __time32_t t; tz(); v32 = INT_MIN; t = v32; CHECK_EQ_I(tc_p__localtime32_s(&tm, &t), EINVAL); }
TC_CASE(gm32_min) { struct tm tm; __time32_t t; v32 = INT_MIN; t = v32; CHECK_EQ_I(tc_p__gmtime32_s(&tm, &t), EINVAL); }
TC_CASE(valid_zero)
{
    struct tm tm; __time64_t t; tz(); v64 = 0; t = v64;
    CHECK_EQ_I(tc_p__localtime64_s(&tm, &t), 0); CHECK_EQ_I(tm.tm_year, 70); CHECK_EQ_I(tm.tm_mon, 0); CHECK_EQ_I(tm.tm_mday, 1); CHECK_EQ_I(tm.tm_hour, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install(); TC_LOAD_PROC(_localtime64_s); TC_LOAD_PROC(_localtime32_s); TC_LOAD_PROC(_gmtime32_s);
    CHECK(tc_p__localtime64_s && tc_p__localtime32_s && tc_p__gmtime32_s); if (!tc_p__localtime64_s || !tc_p__localtime32_s || !tc_p__gmtime32_s) return tc_end();
    TC_RUN(local64_max); TC_RUN(local64_min); TC_RUN(local32_min); TC_RUN(gm32_min); TC_RUN(valid_zero); return tc_end();
}
