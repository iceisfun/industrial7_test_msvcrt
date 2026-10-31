/* TC
 * id:    tm_ctime64_max
 * area:  time
 * funcs: _ctime64
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs ctime/_ctime64: valid range is midnight Jan 1 1970 through 23:59:59 Dec 31 3000 UTC
 * note:  3000-12-31 23:59:59 UTC is 32535215999 (= _MAX__TIME64_T, 0x793406fff in time.h docs).
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

static volatile __time64_t vt;

TC_CASE(last_second)
{
    __time64_t t; char *r; _putenv("TZ=UTC0"); _tzset();
    vt = 32535215999LL; t = vt; r = _ctime64(&t);
    CHECK(r != NULL); if (r) CHECK_STR(r, "Wed Dec 31 23:59:59 3000\n");
}
TC_CASE(one_before)
{
    __time64_t t; char *r; _putenv("TZ=UTC0"); _tzset();
    vt = 32535215998LL; t = vt; r = _ctime64(&t);
    CHECK(r != NULL); if (r) CHECK_STR(r, "Wed Dec 31 23:59:58 3000\n");
}
TC_CASE(year_2038_plus)
{
    __time64_t t; char *r; _putenv("TZ=UTC0"); _tzset();
    vt = 2147483648LL; t = vt; r = _ctime64(&t);
    CHECK(r != NULL); if (r) CHECK_STR(r, "Tue Jan 19 03:14:08 2038\n");
}
int main(void) { tc_begin(); TC_RUN(last_second); TC_RUN(one_before); TC_RUN(year_2038_plus); return tc_end(); }
