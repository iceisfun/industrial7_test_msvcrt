/* TC
 * id:    dat_neg_get_tz_null_params
 * area:  data_exports
 * funcs: _get_daylight _get_timezone _get_dstbias _get_tzname
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _get_daylight/_get_timezone/_get_dstbias/_get_tzname (NULL pointer or index not 0/1 -> invalid parameter handler, errno EINVAL)
 */
#include "tc.h"
#include <errno.h>
#include <time.h>

#define NEG(call) do { int tc_r; tc_iph_count = 0; errno = 0; tc_r = (call); \
    CHECK_EQ_I(tc_r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(daylight_null) { NEG(_get_daylight(NULL)); }
TC_CASE(timezone_null) { NEG(_get_timezone(NULL)); }
TC_CASE(dstbias_null)  { NEG(_get_dstbias(NULL)); }
TC_CASE(tzname_index_bad)
{ char b[16] = "keep"; size_t n = 0; NEG(_get_tzname(&n, b, sizeof b, 2)); NEG(_get_tzname(&n, b, sizeof b, -1)); }
TC_CASE(tzname_null_buffer_nonzero_size) { size_t n = 0; NEG(_get_tzname(&n, NULL, 16, 0)); }
TC_CASE(tzname_null_retval) { char b[16]; NEG(_get_tzname(NULL, b, sizeof b, 0)); }

int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(daylight_null); TC_RUN(timezone_null); TC_RUN(dstbias_null); TC_RUN(tzname_index_bad); TC_RUN(tzname_null_buffer_nonzero_size); TC_RUN(tzname_null_retval);
    return tc_end();
}
