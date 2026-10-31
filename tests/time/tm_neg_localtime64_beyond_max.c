/* TC
 * id:    tm_neg_localtime64_beyond_max
 * area:  time
 * kind:  negative
 * funcs: _localtime64
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _localtime64: range ends at 23:59:59 Dec 31 3000 UTC
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

static volatile __time64_t v_t;

TC_CASE(just_beyond) { struct tm *p; v_t = 32535244800LL; p = _localtime64((const __time64_t *)&v_t); CHECK(p == NULL); }
TC_CASE(llong_max) { struct tm *p; v_t = LLONG_MAX; p = _localtime64((const __time64_t *)&v_t); CHECK(p == NULL); }
TC_CASE(last_valid) { struct tm *p; v_t = 32535215999LL; /* 3000-12-31 23:59:59 UTC */
    p = _localtime64((const __time64_t *)&v_t); CHECK(p != NULL);
    if (p) { CHECK_EQ_I(p->tm_year, 1100); CHECK_EQ_I(p->tm_mon, 11); CHECK_EQ_I(p->tm_mday, 31); CHECK_EQ_I(p->tm_hour, 23); CHECK_EQ_I(p->tm_sec, 59); } }

int main(void)
{
    tc_begin();
    if (TC_AT_LEAST(80)) tc_iph_install();
    _putenv("TZ=UTC0"); _tzset();
    TC_RUN(just_beyond); TC_RUN(llong_max); TC_RUN(last_valid);
    _putenv("TZ=");
    return tc_end();
}
