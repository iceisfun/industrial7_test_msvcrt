/* TC
 * id:    tm_neg_mkgmtime64_range
 * area:  time
 * kind:  negative
 * funcs: _mkgmtime64
 * crt:   >=80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _mkgmtime64: range 1970-01-01 to 3000-12-31 23:59:59 UTC
 * note:  ucrtbase accepts _mkgmtime64 up to 3001-01-19 21:59:59 UTC (32536850399); past_3000 uses 3001-01-19 22:00:00.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static void settz(const char *s) { _putenv(s); _tzset(); }
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s, int dst)
{
    volatile int v = y; t->tm_year = v - 1900; t->tm_mon = mo - 1; t->tm_mday = d;
    t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_isdst = dst; t->tm_wday = 0; t->tm_yday = 0;
}

TC_CASE(pre_epoch) { struct tm t; mk(&t, 1969, 12, 31, 23, 59, 59, 0); CHECK_EQ_I(_mkgmtime64(&t), -1); }
/* observed on Win10 ucrtbase: _mkgmtime64 limit is 32536850399 (3001-01-19 21:59:59 UTC, same as _gmtime64); 3001-01-01 is still accepted */
TC_CASE(past_3000) { struct tm t;
#if TC_CRT_VER >= 140
    mk(&t, 3001, 1, 19, 22, 0, 0, 0);
#else
    mk(&t, 3001, 1, 1, 0, 0, 0, 0);
#endif
    CHECK_EQ_I(_mkgmtime64(&t), -1); }
TC_CASE(last_ok_3001) { struct tm t;
#if TC_CRT_VER >= 140
    mk(&t, 3001, 1, 19, 21, 59, 59, 0); CHECK_EQ_I(_mkgmtime64(&t), 32536850399LL);
#else
    (void)t; TC_SKIP("ucrtbase-only observation");
#endif
}
TC_CASE(year_1900) { struct tm t; mk(&t, 1900, 1, 1, 0, 0, 0, 0); CHECK_EQ_I(_mkgmtime64(&t), -1); }
TC_CASE(boundary) { struct tm t; mk(&t, 3000, 12, 31, 23, 59, 59, 0); CHECK_EQ_I(_mkgmtime64(&t), 32535215999LL); }
int main(void) { tc_begin(); TC_RUN(pre_epoch); TC_RUN(past_3000); TC_RUN(last_ok_3001); TC_RUN(year_1900); TC_RUN(boundary); return tc_end(); }
