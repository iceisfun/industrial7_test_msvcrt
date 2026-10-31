/* TC
 * id:    tm_neg_ctime32_args
 * area:  time
 * kind:  negative
 * funcs: _ctime32
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * note:  errno for the out-of-range cases is not documented; see tm_neg_ctime_probe_range_errno.
 * ref:   MS docs ctime, _ctime32: NULL for a time before midnight Jan 1 1970 UTC; NULL timer invalid parameter
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

static volatile __time32_t vt;

TC_CASE(null_timer)
{
    const __time32_t *volatile p = NULL; char *r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    _putenv("TZ=UTC0"); _tzset(); tc_iph_count = 0; errno = 0;
    r = _ctime32((const __time32_t *)p);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(minus_one)
{
    __time32_t t; char *r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    _putenv("TZ=UTC0"); _tzset(); tc_iph_count = 0; errno = 0;
    vt = -1; t = vt; r = _ctime32(&t);
    CHECK(r == NULL);
}
TC_CASE(int_min)
{
    __time32_t t; char *r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    _putenv("TZ=UTC0"); _tzset(); tc_iph_count = 0; errno = 0;
    vt = INT_MIN; t = vt; r = _ctime32(&t);
    CHECK(r == NULL);
}
int main(void) { tc_begin(); TC_RUN(null_timer); TC_RUN(minus_one); TC_RUN(int_min); return tc_end(); }
