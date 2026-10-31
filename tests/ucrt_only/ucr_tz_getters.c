/* TC
 * id:    ucr_tz_getters
 * area:  ucrt_only
 * funcs: _tzset _get_timezone _get_daylight _get_dstbias _get_tzname
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _get_timezone, _get_daylight, _get_dstbias, _get_tzname, _tzset
 * note:  Values are machine/TZ dependent; only consistency and ranges are checked.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

static size_t slen(const char *s) { size_t n = 0; while (s[n]) n++; return n; }

TC_CASE(returns_zero)
{
    long tz = 123456; int dl = -7; long db = 99999; size_t n = 0; char b[128];
    _tzset();
    CHECK_EQ_I(_get_timezone(&tz), 0);
    CHECK_EQ_I(_get_daylight(&dl), 0);
    CHECK_EQ_I(_get_dstbias(&db), 0);
    CHECK_EQ_I(_get_tzname(&n, b, sizeof b, 0), 0);
}
TC_CASE(ranges_and_repeatable)
{
    long tz = 0, tz2 = 1; int dl = -1, dl2 = -2; long db = 1, db2 = 2;
    _tzset();
    _get_timezone(&tz); _get_daylight(&dl); _get_dstbias(&db);
    _get_timezone(&tz2); _get_daylight(&dl2); _get_dstbias(&db2);
    CHECK_EQ_I(tz, tz2); CHECK_EQ_I(dl, dl2); CHECK_EQ_I(db, db2);
    CHECK(tz >= -14 * 3600 && tz <= 12 * 3600);   /* seconds west of UTC */
    CHECK(dl == 0 || dl == 1);
}
TC_CASE(dstbias_value)
{
    long db = 12345;
    _tzset();
    CHECK_EQ_I(_get_dstbias(&db), 0);
    CHECK(db <= 0 && db >= -3600);   /* -(DaylightBias): 0, or e.g. -1800 (Lord Howe), -3600 */
}
TC_CASE(tzname_std)
{
    size_t n = 0; char b[128]; int i;
    _tzset();
    for (i = 0; i < (int)sizeof b; i++) b[i] = 'Z';
    CHECK_EQ_I(_get_tzname(&n, b, sizeof b, 0), 0);
    CHECK_EQ_U(n, slen(b) + 1);
    CHECK(slen(b) < sizeof b);
}
TC_CASE(tzname_dst)
{
    size_t n = 0; char b[128];
    _tzset();
    b[0] = 0;
    CHECK_EQ_I(_get_tzname(&n, b, sizeof b, 1), 0);
    CHECK_EQ_U(n, slen(b) + 1);
}
TC_CASE(tzname_size_query)
{
    size_t n = 0, m = 0; char b[128];
    _tzset();
    _get_tzname(&n, b, sizeof b, 0);
    CHECK_EQ_I(_get_tzname(&m, NULL, 0, 0), 0);   /* documented: NULL buffer with size 0 only reports the size */
    CHECK_EQ_U(m, n);
}

int main(void) { tc_begin(); TC_RUN(returns_zero); TC_RUN(ranges_and_repeatable); TC_RUN(dstbias_value); TC_RUN(tzname_std); TC_RUN(tzname_dst); TC_RUN(tzname_size_query); return tc_end(); }
