/* TC
 * id:    ucr_neg_tz_getters_small_buf_probe
 * area:  ucrt_only
 * funcs: _get_tzname _tzset
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _get_tzname (buffer too small)
 * note:  PROBE: _get_tzname with a 1-byte buffer (too small for any non-empty zone name) returns ERANGE, sets
 *        errno=ERANGE, invokes the invalid-parameter handler once, and reports the required size (>= 2) in *n.
 *        Each of these (IPH call on ERANGE, size reported on failure) is a guess; separate cases so one wrong
 *        guess does not hide the others.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

static void setup(void) { tc_iph_count = 0; errno = 0; }

TC_CASE(ret_and_errno)
{
    size_t n = 0; char b[4]; _tzset(); setup(); b[0] = 'x';
    CHECK_EQ_I(_get_tzname(&n, b, 1, 0), ERANGE);
    CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(iph_called)
{
    size_t n = 0; char b[4]; _tzset(); setup(); b[0] = 'x';
    _get_tzname(&n, b, 1, 0);
    CHECK_EQ_I(tc_iph_count, 1);
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
    TC_RUN(ret_and_errno); TC_RUN(iph_called); TC_RUN(size_reported);
    return tc_end();
}
