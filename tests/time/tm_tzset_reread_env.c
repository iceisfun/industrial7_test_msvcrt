/* TC
 * id:    tm_tzset_reread_env
 * area:  time
 * funcs: _tzset _get_timezone _localtime64
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _tzset, _localtime64
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

static volatile __time64_t v_t = 86400;
static __time64_t get_t(void) { return v_t; }

TC_CASE(reload)
{
    long tz = 0;
    _putenv("TZ=EST5"); _tzset(); _get_timezone(&tz); CHECK_EQ_I(tz, 18000);
    _putenv("TZ=JST-9"); _tzset(); _get_timezone(&tz); CHECK_EQ_I(tz, -32400);
}
TC_CASE(idempotent)
{
    long a = 0, b = 0;
    _putenv("TZ=EST5"); _tzset(); _get_timezone(&a); _tzset(); _get_timezone(&b);
    CHECK_EQ_I(a, 18000); CHECK_EQ_I(b, 18000);
}
TC_CASE(localtime_follows)
{
    struct tm *r; __time64_t t;
    _putenv("TZ=UTC0"); _tzset(); t = get_t(); r = _localtime64(&t); CHECK(r != NULL);
    if (r) { CHECK_EQ_I(r->tm_mday, 2); CHECK_EQ_I(r->tm_hour, 0); }
    _putenv("TZ=JST-9"); _tzset(); t = get_t(); r = _localtime64(&t); CHECK(r != NULL);
    if (r) { CHECK_EQ_I(r->tm_mday, 2); CHECK_EQ_I(r->tm_hour, 9); }
}
int main(void) { tc_begin(); TC_RUN(reload); TC_RUN(idempotent); TC_RUN(localtime_follows); return tc_end(); }
