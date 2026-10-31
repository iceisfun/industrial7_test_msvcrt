/* TC
 * id:    sec_neg_gmtime32_s_range
 * area:  secure_crt
 * funcs: _gmtime32_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _gmtime32_s: negative time or NULL args -> EINVAL
 * note:  whether the invalid-parameter handler runs for a merely out-of-range (negative) time is not documented, so it is not asserted
 *        here (handler asserted for NULL pointers only).
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
static volatile __time32_t vt;
TC_CASE(minus_one_EINVAL)
{ struct tm tm; __time32_t t; int r; vt = -1; t = vt; tc_iph_count = 0; errno = 0;
  r = _gmtime32_s(&tm, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
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
    TC_RUN(minus_one_EINVAL); TC_RUN(null_tm_EINVAL); TC_RUN(null_time_EINVAL); TC_RUN(handler_counts);
    return tc_end();
}
