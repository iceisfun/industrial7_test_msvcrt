/* TC
 * id:    sec_neg_gmtime64_s_range
 * area:  secure_crt
 * kind:  negative
 * funcs:
 * procs: _gmtime64_s _gmtime32_s _errno
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _gmtime64_s / _gmtime32_s: EINVAL for NULL pointers or time outside 1970-01-01 .. 3000-12-31 23:59:59 UTC (32-bit: up to INT32_MAX)
 */
#include "tc.h"
#include <time.h>

typedef int (__cdecl *gm64_t)(struct tm *, const __int64 *);
typedef int (__cdecl *gm32_t)(struct tm *, const __int32 *);
typedef int *(__cdecl *errno_fn)(void);
static gm64_t p_gm64; static gm32_t p_gm32; static errno_fn p_errno;

TC_CASE(null_tm_einval) { volatile __int64 t = 0; int r; tc_iph_count = 0; r = p_gm64(NULL, (const __int64 *)&t); CHECK_EQ_I(r, 22); CHECK(tc_iph_count >= 1); }
TC_CASE(null_time_einval) { struct tm x; int r; tc_iph_count = 0; r = p_gm64(&x, NULL); CHECK_EQ_I(r, 22); CHECK(tc_iph_count >= 1); }
TC_CASE(negative_time_einval)
{
    struct tm x; volatile __int64 t = -1; int r;
    *p_errno() = 0; r = p_gm64(&x, (const __int64 *)&t); CHECK_EQ_I(r, 22); CHECK_EQ_I(*p_errno(), 22);
}
TC_CASE(beyond_year_3000_einval)
{
    struct tm x; volatile __int64 t = 32535216000LL; int r;   /* 3001-01-01 00:00:00 */
    r = p_gm64(&x, (const __int64 *)&t); CHECK_EQ_I(r, 22);
    t = 0x7fffffffffffffffLL; r = p_gm64(&x, (const __int64 *)&t); CHECK_EQ_I(r, 22);
}
TC_CASE(gmtime32_negative_einval)
{
    struct tm x; volatile __int32 t = -1; int r;
    if (!p_gm32) TC_SKIP("_gmtime32_s not exported");
    r = p_gm32(&x, (const __int32 *)&t); CHECK_EQ_I(r, 22);
    t = (__int32)0x80000000; r = p_gm32(&x, (const __int32 *)&t); CHECK_EQ_I(r, 22);
}
int main(void)
{
    tc_begin();
    p_gm64 = (gm64_t)tc_crt_proc("_gmtime64_s"); p_gm32 = (gm32_t)tc_crt_proc("_gmtime32_s"); p_errno = (errno_fn)tc_crt_proc("_errno");
    CHECK(p_gm64 && p_errno); if (!(p_gm64 && p_errno)) return tc_end();
    tc_iph_install();
    TC_RUN(null_tm_einval); TC_RUN(null_time_einval); TC_RUN(negative_time_einval); TC_RUN(beyond_year_3000_einval); TC_RUN(gmtime32_negative_einval);
    return tc_end();
}
