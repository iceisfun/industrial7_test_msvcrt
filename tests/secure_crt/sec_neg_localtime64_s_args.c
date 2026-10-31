/* TC
 * id:    sec_neg_localtime64_s_args
 * area:  secure_crt
 * funcs: _localtime64_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _localtime64_s: NULL args, negative or beyond Dec 31 3000 (UTC-adjusted) -> EINVAL
 * note:  handler asserted for NULL args only (not documented for out-of-range times).
 *        only -1 and values at least two days beyond the max are asserted so the result is timezone independent */
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
{ struct tm tm; int r = try(&tm, 1, 32535244799LL + 172800LL); CHECK_EQ_I(r, EINVAL); }
TC_CASE(handler_counts)
{ struct tm tm; try(&tm, 0, 0); CHECK(tc_iph_count >= 1); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(null_tm_EINVAL); TC_RUN(null_time_EINVAL); TC_RUN(negative_one_EINVAL);
    TC_RUN(beyond_max_EINVAL); TC_RUN(handler_counts);
    return tc_end();
}
