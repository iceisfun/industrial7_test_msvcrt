/* TC
 * id:    tm_mktime64_gmtime_roundtrip
 * area:  time
 * funcs: _mktime64 _gmtime64
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.27.3.3, 7.27.2.3
 * note:  TZ=UTC0
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

static void settz(const char *s) { _putenv(s); _tzset(); }
static struct tm mk(int y, int mo, int d, int h, int mi, int s, int dst)
{
    struct tm t; memset(&t, 0, sizeof t);
    t.tm_year = y - 1900; t.tm_mon = mo - 1; t.tm_mday = d; t.tm_hour = h; t.tm_min = mi; t.tm_sec = s; t.tm_isdst = dst;
    return t;
}

static const __time64_t ts[] = { 0, 1, 59, 86399, 86400, 951782400LL, 951868799LL, 1000000000LL, 1234567890LL, 1500000000LL,
    1600000000LL, 1700000000LL, 2000000000LL, 2147483647LL, 2147483648LL, 3000000000LL, 4102444800LL, 4294967295LL, 10000000000LL, 32503680000LL };
TC_CASE(sweep)
{
    size_t i; settz("TZ=UTC0");
    for (i = 0; i < sizeof ts / sizeof ts[0]; i++) {
        volatile __time64_t t = ts[i]; struct tm *g = _gmtime64((const __time64_t *)&t); struct tm c;
        CHECK(g != NULL); if (!g) continue;
        c = *g;
        CHECK_EQ_I(_mktime64(&c), ts[i]);
    }
}
TC_CASE(near_max)
{
    volatile __time64_t t = 32535215999LL; struct tm *g; struct tm c;
    settz("TZ=UTC0");
    g = _gmtime64((const __time64_t *)&t); CHECK(g != NULL); if (!g) return;
    CHECK_EQ_I(g->tm_year, 1100); CHECK_EQ_I(g->tm_mon, 11); CHECK_EQ_I(g->tm_mday, 31); CHECK_EQ_I(g->tm_hour, 23);
    c = *g; CHECK_EQ_I(_mktime64(&c), 32535215999LL);
}
int main(void) { tc_begin(); TC_RUN(sweep); TC_RUN(near_max); return tc_end(); }
