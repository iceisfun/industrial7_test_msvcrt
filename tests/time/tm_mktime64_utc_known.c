/* TC
 * id:    tm_mktime64_utc_known
 * area:  time
 * funcs: _mktime64
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mktime64; C11 7.27.2.3
 * note:  TZ=UTC0 so local time == UTC
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

TC_CASE(known)
{
    struct tm a, b, c;
    settz("TZ=UTC0");
    a = mk(1970, 1, 1, 0, 0, 0, 0);   CHECK_EQ_I(_mktime64(&a), 0);
    b = mk(2000, 2, 29, 0, 0, 0, 0);  CHECK_EQ_I(_mktime64(&b), 951782400LL);
    c = mk(2001, 9, 9, 1, 46, 40, 0); CHECK_EQ_I(_mktime64(&c), 1000000000LL);
}
TC_CASE(isdst_minus1_same)
{
    struct tm a = mk(2001, 9, 9, 1, 46, 40, -1), b = mk(2001, 9, 9, 1, 46, 40, 0);
    settz("TZ=UTC0");
    CHECK_EQ_I(_mktime64(&a), 1000000000LL);
    CHECK_EQ_I(_mktime64(&b), 1000000000LL);
    CHECK_EQ_I(a.tm_isdst, 0);
}
int main(void) { tc_begin(); TC_RUN(known); TC_RUN(isdst_minus1_same); return tc_end(); }
