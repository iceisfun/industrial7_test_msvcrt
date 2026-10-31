/* TC
 * id:    sec_neg_gmtime64_s_args
 * area:  secure_crt
 * funcs: _gmtime64_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _gmtime64_s: NULL tmDest or sourceTime -> EINVAL
 * note:  contents of *tm after NULL sourceTime failure are not asserted.
 *        Handler call counts are asserted as >=1 per failure (exact count is an implementation detail).
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
static volatile __time64_t vt = 100;
TC_CASE(null_tm_EINVAL)
{
    __time64_t t = vt; int r; tc_iph_count = 0; errno = 0;
    r = _gmtime64_s(NULL, &t);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(null_time_EINVAL)
{
    struct tm tm; int r; memset(&tm, 0x5a, sizeof tm); tc_iph_count = 0; errno = 0;
    r = _gmtime64_s(&tm, NULL);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(handler_counts)
{
    struct tm tm; __time64_t t = vt; tc_iph_count = 0;
    int n;
    _gmtime64_s(NULL, &t); CHECK(tc_iph_count >= 1);
    _gmtime64_s(&tm, NULL); CHECK(tc_iph_count >= 2);
    n = tc_iph_count;
    CHECK_EQ_I(_gmtime64_s(&tm, &t), 0);
    CHECK_EQ_I(tc_iph_count, n);
    CHECK_EQ_I(tm.tm_sec, 40); CHECK_EQ_I(tm.tm_min, 1); CHECK_EQ_I(tm.tm_year, 70);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(null_tm_EINVAL); TC_RUN(null_time_EINVAL); TC_RUN(handler_counts);
    return tc_end();
}
