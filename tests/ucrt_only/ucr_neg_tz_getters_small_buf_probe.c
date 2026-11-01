/* TC
 * id:    ucr_neg_tz_getters_small_buf_probe
 * area:  ucrt_only
 * funcs: _get_tzname _tzset
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _get_tzname (buffer too small)
 * note:  Win10 22H2 (observed): _get_tzname with a 1-byte buffer returns ERANGE, reports the required size (>= 2; 22 for "Pacific Standard Time", time-zone dependent so only >= 2 is asserted),
 *        sets buffer[0]=0, leaves errno untouched and does not call the invalid-parameter handler.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

static void setup(void) { tc_iph_count = 0; errno = 0; }

TC_CASE(ret_and_errno)
{
    size_t n = 0; char b[4]; _tzset(); setup(); b[0] = 'x';
    CHECK_EQ_I(_get_tzname(&n, b, 1, 0), ERANGE);
    CHECK_EQ_I(errno, 0);   /* observed on Win10 22H2: ERANGE is returned but errno is left untouched */
}
TC_CASE(iph_not_called)
{
    size_t n = 0; char b[4]; _tzset(); setup(); b[0] = 'x';
    _get_tzname(&n, b, 1, 0);
    CHECK_EQ_I(tc_iph_count, 0);   /* observed: no handler call */
}
TC_CASE(size_reported)
{
    size_t n = 0; char b[4]; _tzset(); setup(); b[0] = 'x';
    _get_tzname(&n, b, 1, 0);
    CHECK(n >= 2);
}

int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(ret_and_errno); TC_RUN(iph_not_called); TC_RUN(size_reported);
    return tc_end();
}
