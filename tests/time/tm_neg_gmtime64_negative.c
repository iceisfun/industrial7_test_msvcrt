/* TC
 * id:    tm_neg_gmtime64_negative
 * area:  time
 * funcs: _gmtime64
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _gmtime64: NULL if before midnight Jan 1 1970
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

static volatile __time64_t v_t;

static void one(__time64_t t)
{
    struct tm *r; v_t = t; errno = 0; tc_iph_count = 0;
    r = _gmtime64((const __time64_t *)&v_t);
    CHECK(r == NULL);
#if TC_CRT_VER >= 90
    CHECK_EQ_I(errno, EINVAL);
#endif
}
TC_CASE(minus_one) { one(-1); }
TC_CASE(minus_day) { one(-86400); }
TC_CASE(llong_min) { one(LLONG_MIN); }
TC_CASE(zero_still_valid) { struct tm *r; v_t = 0; r = _gmtime64((const __time64_t *)&v_t); CHECK(r != NULL); if (r) CHECK_EQ_I(r->tm_year, 70); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(minus_one); TC_RUN(minus_day); TC_RUN(llong_min); TC_RUN(zero_still_valid); return tc_end(); }
