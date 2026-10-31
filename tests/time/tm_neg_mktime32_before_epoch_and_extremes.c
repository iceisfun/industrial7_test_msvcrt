/* TC
 * id:    tm_neg_mktime32_before_epoch_and_extremes
 * area:  time
 * kind:  negative
 * funcs: _mktime32 _mkgmtime32
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs mktime, _mktime32, _mkgmtime32: -1 if the date is before midnight Jan 1 1970 UTC or after 03:14:07 Jan 19 2038 UTC
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <limits.h>
static volatile int vi;
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s)
{ t->tm_year = y; t->tm_mon = mo; t->tm_mday = d; t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_wday = 0; t->tm_yday = 0; t->tm_isdst = 0; }
static void tz(void) { _putenv("TZ=UTC0"); _tzset(); }
TC_CASE(year_max) { struct tm t; tz(); mk(&t, 100, 0, 1, 0, 0, 0); vi = INT_MAX; t.tm_year = vi; CHECK_EQ_I(_mktime32(&t), -1); }
TC_CASE(sec_min) { struct tm t; tz(); mk(&t, 100, 0, 1, 0, 0, 0); vi = INT_MIN; t.tm_sec = vi; CHECK_EQ_I(_mktime32(&t), -1); }
TC_CASE(one_before_epoch) { struct tm t; tz(); mk(&t, 69, 11, 31, 23, 59, 59); CHECK_EQ_I(_mktime32(&t), -1); }
TC_CASE(epoch) { struct tm t; tz(); mk(&t, 70, 0, 1, 0, 0, 0); CHECK_EQ_I(_mktime32(&t), 0); }
TC_CASE(gm_year_max) { struct tm t; mk(&t, 100, 0, 1, 0, 0, 0); vi = INT_MAX; t.tm_year = vi; CHECK_EQ_I(_mkgmtime32(&t), -1); }
TC_CASE(gm_before_epoch) { struct tm t; mk(&t, 69, 11, 31, 23, 59, 59); CHECK_EQ_I(_mkgmtime32(&t), -1); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(year_max); TC_RUN(sec_min); TC_RUN(one_before_epoch); TC_RUN(epoch); TC_RUN(gm_year_max); TC_RUN(gm_before_epoch); return tc_end(); }
