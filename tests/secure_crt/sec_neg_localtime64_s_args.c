/* TC
 * id:    sec_neg_localtime64_s_args
 * area:  secure_crt
 * funcs: _localtime64_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _localtime64_s: NULL args, negative or beyond Dec 31 3000 (UTC-adjusted) -> EINVAL
 * note:  null_tm/null_time/-1 EINVAL are MS-documented (_localtime64_s). Observed on Win10 22H2 19045 ucrtbase: the upper limit
 *        is about 18 days past 3000-12-31 23:59:59 UTC (32535417599 = old test value was ACCEPTED, returned 0), so the beyond-
 *        max input is now 30 days past 32535215999. Slack at the limit is an implementation detail and may depend on the time
 *        zone (VM zone not pinned: flagged).
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
static volatile __time64_t vt;
static int try(struct tm *tm, int usetime, __time64_t v)
{ __time64_t t; vt = v; t = vt; tc_iph_count = 0; errno = 0; return _localtime64_s(tm, usetime ? &t : NULL); }
TC_CASE(null_tm_EINVAL)
{ __time64_t t = 5; int r; vt = 5; t = vt; tc_iph_count = 0; errno = 0;
  r = _localtime64_s(NULL, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(null_time_EINVAL)
{ struct tm tm; int r = try(&tm, 0, 0); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(negative_one_EINVAL)
{ struct tm tm; int r = try(&tm, 1, -1); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(beyond_max_EINVAL)
{ struct tm tm; int r = try(&tm, 1, 32535215999LL + 30LL * 86400LL); CHECK_EQ_I(r, EINVAL); }   /* observed: ~18 days of slack past 3000-12-31, +30 days is rejected */
TC_CASE(handler_counts)
{ struct tm tm; try(&tm, 0, 0); CHECK(tc_iph_count >= 1); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(null_tm_EINVAL); TC_RUN(null_time_EINVAL); TC_RUN(negative_one_EINVAL);
    TC_RUN(beyond_max_EINVAL); TC_RUN(handler_counts);
    return tc_end();
}
