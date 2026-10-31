/* TC
 * id:    tm_neg_mkgmtime64_extreme_fields
 * area:  time
 * kind:  negative
 * funcs: _mkgmtime64
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mkgmtime, _mkgmtime64: returns -1 if the date cannot be represented
 * note:  only year extremes and tm_sec INT_MIN (lands in 1931). Extreme mon/mday/hour/min normalisation is in
 *        tm_mktime64_probe_extreme_fields. tm_sec INT_MAX omitted (representable, see tm_mktime64_probe_sec_int_max).
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <limits.h>
static volatile int vi;
static struct tm t;
static void good(void) { t.tm_year = 100; t.tm_mon = 0; t.tm_mday = 1; t.tm_hour = 12; t.tm_min = 0; t.tm_sec = 0; t.tm_wday = 0; t.tm_yday = 0; t.tm_isdst = 0; }
TC_CASE(int_max_year) { good(); vi = INT_MAX; t.tm_year = vi; CHECK_EQ_I(_mkgmtime64(&t), -1); }
TC_CASE(int_min_year) { good(); vi = INT_MIN; t.tm_year = vi; CHECK_EQ_I(_mkgmtime64(&t), -1); }
TC_CASE(int_min_sec) { good(); vi = INT_MIN; t.tm_sec = vi; CHECK_EQ_I(_mkgmtime64(&t), -1); }
TC_CASE(valid_after) { good(); CHECK_EQ_I(_mkgmtime64(&t), 946728000LL); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(int_max_year); TC_RUN(int_min_year); TC_RUN(int_min_sec); TC_RUN(valid_after);
    return tc_end();
}
