/* TC
 * id:    tm_neg_probe_mktime_failure_fields
 * area:  time
 * kind:  negative
 * funcs: _mktime64
 * crt:   >=90
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _mktime64 (returns -1 if out of range)
 * note:  Probes what a failed _mktime64 (tm_year 1101 = year 3001, TZ=UTC0) does beyond returning -1: (a) tm_wday/tm_yday
 *        are left untouched, (b) errno becomes EINVAL, (c) the invalid parameter handler is NOT called.
 *        Win10 19045 ucrtbase accepts early January 3001, so the failing input is 3001-12-31 00:00:00; observed: -1, errno EINVAL, tm_wday/tm_yday untouched, handler not called.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>

static void setup(struct tm *t)
{
    _putenv("TZ=UTC0"); _tzset();
    t->tm_year = 1101;
#if TC_CRT_VER >= 140
    t->tm_mon = 11; t->tm_mday = 31;   /* 3001-12-31: 3001-01-01 is accepted by ucrtbase (limit 3001-01-19 07:59:59 UTC) */
#else
    t->tm_mon = 0; t->tm_mday = 1;
#endif
    t->tm_hour = 0; t->tm_min = 0; t->tm_sec = 0;
    t->tm_wday = 3; t->tm_yday = 77; t->tm_isdst = 0;
}
TC_CASE(fields_unchanged) { struct tm t; __time64_t r; setup(&t); r = _mktime64(&t); CHECK_EQ_I(r, -1); CHECK_EQ_I(t.tm_wday, 3); CHECK_EQ_I(t.tm_yday, 77); CHECK_EQ_I(t.tm_year, 1101); }
TC_CASE(errno_einval) { struct tm t; setup(&t); errno = 0; CHECK_EQ_I(_mktime64(&t), -1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(iph_not_called) { struct tm t; setup(&t); tc_iph_count = 0; CHECK_EQ_I(_mktime64(&t), -1); CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(fields_unchanged); TC_RUN(errno_einval); TC_RUN(iph_not_called); _putenv("TZ="); return tc_end(); }
