/* TC
 * id:    tm_mkgmtime64_basic
 * area:  time
 * kind:  positive
 * funcs: _mkgmtime64
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mkgmtime, _mkgmtime32, _mkgmtime64
 * note:  value/TZ-independence only; isdst handling is in tm_mkgmtime64_probe_fields
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

TC_CASE(pst) { struct tm t; settz("TZ=PST8PDT"); mk(&t, 2000, 2, 29, 0, 0, 0, 0); CHECK_EQ_I(_mkgmtime64(&t), 951782400); }
TC_CASE(jst) { struct tm t; settz("TZ=JST-9"); mk(&t, 2000, 2, 29, 0, 0, 0, 0); CHECK_EQ_I(_mkgmtime64(&t), 951782400); }
TC_CASE(epoch) { struct tm t; settz("TZ=JST-9"); mk(&t, 1970, 1, 1, 0, 0, 0, 0); CHECK_EQ_I(_mkgmtime64(&t), 0); }
int main(void) { tc_begin(); TC_RUN(pst); TC_RUN(jst); TC_RUN(epoch); settz("TZ="); return tc_end(); }
