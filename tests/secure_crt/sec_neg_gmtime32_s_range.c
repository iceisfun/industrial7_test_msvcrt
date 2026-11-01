/* TC
 * id:    sec_neg_gmtime32_s_range
 * area:  secure_crt
 * funcs: _gmtime32_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _gmtime32_s: negative time or NULL args -> EINVAL
 * note:  [observed on ucrtbase 19045 only; the same expectation on the legacy CRTs listed in crt: is [unverified]] OBSERVED on Win10 22H2 (19045): _gmtime32_s(-1) and (-43200) return 0 (a 12 h tolerance before the epoch exists; the
 *        tm contents for -1 are even odd, not asserted), while -86400 and INT32_MIN return EINVAL; NULL pointers still return
 *        EINVAL with a handler call. The MS docs say any negative value is EINVAL; Windows deviates. The old minus_one_EINVAL
 *        case is replaced by minus_86400_EINVAL (EINVAL, errno EINVAL) and minus_one_accepted. Deterministic (pure arithmetic,
 *        no time zone).
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
static volatile __time32_t vt;
TC_CASE(minus_86400_EINVAL)
{ struct tm tm; __time32_t t; int r; vt = -86400; t = vt; tc_iph_count = 0; errno = 0;
  r = _gmtime32_s(&tm, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(minus_one_accepted)   /* observed: values down to -43200 (12 h before the epoch) are accepted, so -1 is not an error */
{ struct tm tm; __time32_t t; int r; vt = -1; t = vt; tc_iph_count = 0; errno = 0;
  r = _gmtime32_s(&tm, &t); CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, 0); }
TC_CASE(null_tm_EINVAL)
{ __time32_t t; int r; vt = 5; t = vt; tc_iph_count = 0; errno = 0;
  r = _gmtime32_s(NULL, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(null_time_EINVAL)
{ struct tm tm; int r; tc_iph_count = 0; errno = 0;
  r = _gmtime32_s(&tm, NULL); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(handler_counts)
{ struct tm tm; __time32_t t; vt = 5; t = vt; tc_iph_count = 0;
  _gmtime32_s(NULL, &t); CHECK(tc_iph_count >= 1);
  _gmtime32_s(&tm, NULL); CHECK(tc_iph_count >= 2); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(minus_86400_EINVAL); TC_RUN(minus_one_accepted); TC_RUN(null_tm_EINVAL); TC_RUN(null_time_EINVAL); TC_RUN(handler_counts);
    return tc_end();
}
