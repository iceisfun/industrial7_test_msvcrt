/* TC
 * id:    tm_neg_mktime64_extreme_fields
 * area:  time
 * kind:  negative
 * funcs: _mktime64
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs mktime, _mktime64: returns -1 if the calendar time cannot be represented (range 1970 .. 23:59:59 Dec 31 3000 UTC)
 * note:  only tm_year extremes (rejected by the documented year range check) and tm_sec INT_MIN (lands in 1931). Extreme
 *        tm_mon/mday/hour/min normalisation is implementation-defined arithmetic and lives in tm_mktime64_probe_extreme_fields.
 *        tm_sec INT_MAX is absent: it is representable (see tm_mktime64_probe_sec_int_max). TZ=UTC0.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <limits.h>
static volatile int vi;
static struct tm t;
static void good(void) { t.tm_year = 100; t.tm_mon = 0; t.tm_mday = 1; t.tm_hour = 12; t.tm_min = 0; t.tm_sec = 0; t.tm_wday = 0; t.tm_yday = 0; t.tm_isdst = 0; }
static void setup(void) { _putenv("TZ=UTC0"); _tzset(); good(); }
TC_CASE(int_max_year) { setup(); vi = INT_MAX; t.tm_year = vi; CHECK_EQ_I(_mktime64(&t), -1); }
TC_CASE(int_min_year) { setup(); vi = INT_MIN; t.tm_year = vi; CHECK_EQ_I(_mktime64(&t), -1); }
/* 2000-01-01 12:00 minus 2^31 s is about 1931: before 1970, representable in 64-bit arithmetic */
TC_CASE(int_min_sec) { setup(); vi = INT_MIN; t.tm_sec = vi; CHECK_EQ_I(_mktime64(&t), -1); }
TC_CASE(valid_after) { __time64_t r; setup(); r = _mktime64(&t); CHECK_EQ_I(r, 946728000LL); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(int_max_year); TC_RUN(int_min_year); TC_RUN(int_min_sec); TC_RUN(valid_after);
    return tc_end();
}
