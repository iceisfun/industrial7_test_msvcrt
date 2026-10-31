/* TC
 * id:    tm_localtime64_utc_tz
 * area:  time
 * funcs: _localtime64 _gmtime64
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _tzset, _localtime64
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

static volatile __time64_t v_t;
static void set_tz(const char *s) { _putenv(s); _tzset(); }
#define CHK_TM(r, Y, Mo, D, H, Mi, S, DST) do { CHECK((r) != NULL); if (r) { CHECK_EQ_I((r)->tm_year, (Y) - 1900); CHECK_EQ_I((r)->tm_mon, (Mo) - 1); CHECK_EQ_I((r)->tm_mday, D); CHECK_EQ_I((r)->tm_hour, H); CHECK_EQ_I((r)->tm_min, Mi); CHECK_EQ_I((r)->tm_sec, S); CHECK_EQ_I((r)->tm_isdst, DST); } } while (0)

TC_CASE(equals_gmtime)
{
    static const __time64_t ts[] = { 0, 86400 * 10, 1000000000, 1579118400, 2000000000 };
    size_t i;
    set_tz("TZ=UTC0");
    for (i = 0; i < sizeof ts / sizeof ts[0]; i++) {
        struct tm a, *p; int dst;
        v_t = ts[i];
        p = _localtime64((const __time64_t *)&v_t); CHECK(p != NULL); if (!p) continue;
        a = *p; dst = a.tm_isdst;
        p = _gmtime64((const __time64_t *)&v_t); CHECK(p != NULL); if (!p) continue;
        CHECK_EQ_I(a.tm_year, p->tm_year); CHECK_EQ_I(a.tm_mon, p->tm_mon); CHECK_EQ_I(a.tm_mday, p->tm_mday);
        CHECK_EQ_I(a.tm_hour, p->tm_hour); CHECK_EQ_I(a.tm_min, p->tm_min); CHECK_EQ_I(a.tm_sec, p->tm_sec);
        CHECK_EQ_I(a.tm_wday, p->tm_wday); CHECK_EQ_I(a.tm_yday, p->tm_yday);
        CHECK_EQ_I(dst, 0);
    }
}
TC_CASE(max_valid)
{
    struct tm *r;
    set_tz("TZ=UTC0");
    v_t = 32535215999LL; r = _localtime64((const __time64_t *)&v_t);
    CHK_TM(r, 3000, 12, 31, 23, 59, 59, 0);
}
int main(void) { tc_begin(); TC_RUN(equals_gmtime); TC_RUN(max_valid); return tc_end(); }
