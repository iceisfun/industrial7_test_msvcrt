/* TC
 * id:    tm_neg_localtime64_negative
 * area:  time
 * kind:  negative
 * funcs: _localtime64
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _localtime64: returns NULL for dates before midnight Jan 1 1970 UTC
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

static volatile __time64_t v_t;

TC_CASE(minus_one) { struct tm *p; _putenv("TZ=UTC0"); _tzset(); v_t = -1; errno = 0;
    p = _localtime64((const __time64_t *)&v_t); CHECK(p == NULL); if (TC_AT_LEAST(90)) CHECK_EQ_I(errno, EINVAL); }
TC_CASE(minus_year) { struct tm *p; _putenv("TZ=UTC0"); _tzset(); v_t = -86400LL * 365; errno = 0;
    p = _localtime64((const __time64_t *)&v_t); CHECK(p == NULL); if (TC_AT_LEAST(90)) CHECK_EQ_I(errno, EINVAL); }
TC_CASE(llong_min) { struct tm *p; _putenv("TZ=UTC0"); _tzset(); v_t = LLONG_MIN; errno = 0;
    p = _localtime64((const __time64_t *)&v_t); CHECK(p == NULL); if (TC_AT_LEAST(90)) CHECK_EQ_I(errno, EINVAL); }

int main(void)
{
    tc_begin();
    if (TC_AT_LEAST(80)) tc_iph_install();
    TC_RUN(minus_one); TC_RUN(minus_year); TC_RUN(llong_min);
    _putenv("TZ=");
    return tc_end();
}
