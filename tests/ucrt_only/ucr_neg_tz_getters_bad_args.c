/* TC
 * id:    ucr_neg_tz_getters_bad_args
 * area:  ucrt_only
 * funcs: _get_timezone _get_daylight _get_dstbias _get_tzname
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _get_timezone, _get_daylight, _get_dstbias, _get_tzname (EINVAL)
 * note:  NULL out-pointers and bad index only; the ERANGE (buffer too small) path is in ucr_neg_tz_getters_small_buf_probe.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

static void setup(void) { tc_iph_count = 0; errno = 0; }

TC_CASE(timezone_null) { setup(); CHECK_EQ_I(_get_timezone(NULL), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(daylight_null) { setup(); CHECK_EQ_I(_get_daylight(NULL), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(dstbias_null)  { setup(); CHECK_EQ_I(_get_dstbias(NULL), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(tzname_bad_index)
{
    size_t n = 77; char b[64]; setup();
    CHECK_EQ_I(_get_tzname(&n, b, sizeof b, 2), EINVAL);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    setup();
    CHECK_EQ_I(_get_tzname(&n, b, sizeof b, -1), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(tzname_null_size_ptr)
{
    char b[64]; setup();
    CHECK_EQ_I(_get_tzname(NULL, b, sizeof b, 0), EINVAL);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(timezone_null); TC_RUN(daylight_null); TC_RUN(dstbias_null);
    TC_RUN(tzname_bad_index); TC_RUN(tzname_null_size_ptr);
    return tc_end();
}
