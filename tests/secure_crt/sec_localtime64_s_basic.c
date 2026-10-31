/* TC
 * id:    sec_localtime64_s_basic
 * area:  secure_crt
 * funcs:
 * procs: _localtime64_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _localtime64_s
 * note:  Timezone-independent assertions only. Timestamps are far from the 1970 / 3000 range limits (local time may shift them).
 */
#include "tc.h"
#include <time.h>

typedef int (__cdecl *lt_t)(struct tm *, const __int64 *);
typedef int (__cdecl *gm_t)(struct tm *, const __int64 *);
static lt_t p_lt; static gm_t p_gm;

static long long days_from_civil(long long y, int m, int d)
{
    long long era, yoe, doy, doe;
    y -= m <= 2; era = (y >= 0 ? y : y - 399) / 400; yoe = y - era * 400;
    doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1; doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + doe - 719468;
}
static long long secs_of(const struct tm *t)
{
    return days_from_civil(t->tm_year + 1900, t->tm_mon + 1, t->tm_mday) * 86400LL + t->tm_hour * 3600 + t->tm_min * 60 + t->tm_sec;
}
TC_CASE(field_ranges_valid_for_t_86400x400)
{
    int i;
    for (i = 0; i < 40; i++) {
        volatile __int64 t = 86400LL * (400 + i * 137) + i * 997; struct tm x;
        CHECK_EQ_I(p_lt(&x, (const __int64 *)&t), 0);
        CHECK(x.tm_sec >= 0 && x.tm_sec <= 59); CHECK(x.tm_min >= 0 && x.tm_min <= 59);
        CHECK(x.tm_hour >= 0 && x.tm_hour <= 23); CHECK(x.tm_mday >= 1 && x.tm_mday <= 31);
        CHECK(x.tm_mon >= 0 && x.tm_mon <= 11); CHECK(x.tm_wday >= 0 && x.tm_wday <= 6);
        CHECK(x.tm_yday >= 0 && x.tm_yday <= 365); CHECK(x.tm_year >= 70);
        CHECK(x.tm_isdst == 0 || x.tm_isdst == 1);
    }
}
TC_CASE(successive_seconds_increment)
{
    volatile __int64 t = 86400LL * 400 + 10 * 3600 + 30 * 60 + 15; __int64 t2; struct tm a, b;
    CHECK_EQ_I(p_lt(&a, (const __int64 *)&t), 0);
    t2 = t + 1; CHECK_EQ_I(p_lt(&b, &t2), 0);
    /* 10:30:15 + 1s never crosses a minute boundary, nor a DST switch at whole-hour/half-hour boundaries */
    CHECK_EQ_I(b.tm_sec, a.tm_sec + 1);
    CHECK_EQ_I(b.tm_min, a.tm_min); CHECK_EQ_I(b.tm_hour, a.tm_hour); CHECK_EQ_I(b.tm_mday, a.tm_mday);
}
TC_CASE(local_minus_gm_is_whole_quarter_hours_and_under_26h)
{
    int i;
    for (i = 0; i < 12; i++) {
        volatile __int64 t = 86400LL * (500 + i * 300) + 12345; struct tm l, g; long long d;
        CHECK_EQ_I(p_lt(&l, (const __int64 *)&t), 0); CHECK_EQ_I(p_gm(&g, (const __int64 *)&t), 0);
        d = secs_of(&l) - secs_of(&g);
        CHECK(d > -26 * 3600 && d < 26 * 3600);
        CHECK_EQ_I(d % 900, 0);
    }
}
TC_CASE(returns_0_and_fills_tm)
{
    volatile __int64 t = 86400LL * 10000; struct tm x; x.tm_year = -999; x.tm_mday = -999;
    CHECK_EQ_I(p_lt(&x, (const __int64 *)&t), 0);
    CHECK(x.tm_year != -999); CHECK(x.tm_mday != -999);
    CHECK(x.tm_year >= 97 && x.tm_year <= 98);   /* day 10000 = 1997-05-19 */
}
int main(void)
{
    tc_begin();
    p_lt = (lt_t)tc_crt_proc("_localtime64_s"); p_gm = (gm_t)tc_crt_proc("_gmtime64_s");
    CHECK(p_lt && p_gm); if (!(p_lt && p_gm)) return tc_end();
    TC_RUN(field_ranges_valid_for_t_86400x400); TC_RUN(successive_seconds_increment);
    TC_RUN(local_minus_gm_is_whole_quarter_hours_and_under_26h); TC_RUN(returns_0_and_fills_tm);
    return tc_end();
}
