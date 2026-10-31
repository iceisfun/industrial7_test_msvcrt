/* TC
 * id:    tm_mktime64_probe_isdst_forced
 * area:  time
 * funcs: _mktime64 _localtime64
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.27.2.3; MS docs mktime
 * note:  PROBES whether a positive tm_isdst in winter (and zero in summer) is honoured by shifting the result by one hour, as the C standard describes, versus being normalised away. Also probes that tm_isdst/tm_hour after the call equal what _localtime64 reports for the returned time
 *        (i.e. mktime renormalises the struct to the real local time, so a forced tm_isdst=1 in winter comes back as 0 and 11:00).
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

TC_CASE(winter_forced_dst)
{
    struct tm a = mk(2020, 1, 15, 12, 0, 0, 0), b = mk(2020, 1, 15, 12, 0, 0, 1); __time64_t ta, tb;
    settz("TZ=PST8PDT");
    ta = _mktime64(&a); tb = _mktime64(&b);
    CHECK_EQ_I(ta - tb, 3600);
}
TC_CASE(summer_forced_std)
{
    struct tm a = mk(2020, 7, 4, 12, 0, 0, -1), b = mk(2020, 7, 4, 12, 0, 0, 0); __time64_t ta, tb;
    settz("TZ=PST8PDT");
    ta = _mktime64(&a); tb = _mktime64(&b);
    CHECK_EQ_I(tb - ta, 3600);
}
TC_CASE(isdst_after_call)
{
    struct tm b = mk(2020, 1, 15, 12, 0, 0, 1);
    __time64_t r; struct tm *l;
    settz("TZ=PST8PDT");
    r = _mktime64(&b);
    CHECK(r != -1);
    l = _localtime64(&r); CHECK(l != NULL); if (!l) return;
    CHECK_EQ_I(b.tm_isdst, l->tm_isdst);
    CHECK_EQ_I(b.tm_hour, l->tm_hour);
}
int main(void) { tc_begin(); TC_RUN(winter_forced_dst); TC_RUN(summer_forced_std); TC_RUN(isdst_after_call); return tc_end(); }
