/* TC
 * id:    tm_neg_gmtime64_beyond_max
 * area:  time
 * funcs: _gmtime64
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _gmtime64: valid through 23:59:59 Dec 31 3000 UTC
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

static volatile __time64_t v_t;

static void bad(__time64_t t)
{
    struct tm *r; v_t = t; errno = 0;
    r = _gmtime64((const __time64_t *)&v_t);
    CHECK(r == NULL);
#if TC_CRT_VER >= 90
    CHECK_EQ_I(errno, EINVAL);
#endif
}
TC_CASE(just_past) { bad(32535244800LL); }
TC_CASE(llong_max) { bad(LLONG_MAX); }
TC_CASE(boundary_valid)
{
    struct tm *r; v_t = 32535215999LL; r = _gmtime64((const __time64_t *)&v_t);
    CHECK(r != NULL);
    if (r) {
        CHECK_EQ_I(r->tm_year, 1100); CHECK_EQ_I(r->tm_mon, 11); CHECK_EQ_I(r->tm_mday, 31);
        CHECK_EQ_I(r->tm_hour, 23); CHECK_EQ_I(r->tm_min, 59); CHECK_EQ_I(r->tm_sec, 59);
        CHECK_EQ_I(r->tm_yday, 364);
    }
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(just_past); TC_RUN(llong_max); TC_RUN(boundary_valid); return tc_end(); }
