/* TC
 * id:    sec_neg_gmtime64_s_range
 * area:  secure_crt
 * kind:  negative
 * funcs:
 * procs: _gmtime64_s _gmtime32_s _errno
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _gmtime64_s / _gmtime32_s: EINVAL for NULL pointers or time outside 1970-01-01 .. 3000-12-31 23:59:59 UTC (32-bit: up to INT32_MAX)
 * note:  [observed on ucrtbase 19045 only; the same expectation on the legacy CRTs listed in crt: is [unverified]] OBSERVED on Win10 22H2 (19045): _gmtime64_s accepts -43200 <= t <= 32536850399 (3001-01-19 21:59:59 UTC) and returns
 *        EINVAL (errno EINVAL) outside it; so t=-1 succeeds and 3001-01-01 succeeds, contrary to the MS docs (negative / after
 *        3000-12-31 23:59:59 = EINVAL). Cases now use -86400, -43201, 32536850400, 10000-01-01 and INT64_MAX for EINVAL, and
 *        pin the observed accepted edge. _gmtime32_s: -86400 and INT32_MIN return EINVAL. NULL pointer cases (handler called)
 *        unchanged. Pure arithmetic, no time zone/locale/CPU dependence; build-dependent limit [not verified on other builds].
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
    struct tm x; volatile __int64 t = -86400; int r;   /* observed: -43200 is the lowest accepted value (see minus_one_accepted) */
    *p_errno() = 0; r = p_gm64(&x, (const __int64 *)&t); CHECK_EQ_I(r, 22); CHECK_EQ_I(*p_errno(), 22);
}
TC_CASE(minus_one_accepted)
{
    struct tm x; volatile __int64 t = -1; int r;
    *p_errno() = 0; r = p_gm64(&x, (const __int64 *)&t); CHECK_EQ_I(r, 0); CHECK_EQ_I(*p_errno(), 0);
    CHECK_EQ_I(x.tm_year, 69); CHECK_EQ_I(x.tm_sec, 59);
    t = -43200; CHECK_EQ_I(p_gm64(&x, (const __int64 *)&t), 0);
    t = -43201; CHECK_EQ_I(p_gm64(&x, (const __int64 *)&t), 22);
}
TC_CASE(beyond_year_3000_einval)
{
    struct tm x; volatile __int64 t = 32536850400LL; int r;   /* observed first rejected value: 3001-01-19 22:00:00 UTC */
    r = p_gm64(&x, (const __int64 *)&t); CHECK_EQ_I(r, 22);
    t = 253402300800LL; r = p_gm64(&x, (const __int64 *)&t); CHECK_EQ_I(r, 22);   /* 10000-01-01 */
    t = 0x7fffffffffffffffLL; r = p_gm64(&x, (const __int64 *)&t); CHECK_EQ_I(r, 22);
}
TC_CASE(year_3000_limit_is_loose)   /* observed: 3001-01-01 (docs: limit 3000-12-31 23:59:59) is still accepted, up to 32536850399 */
{
    struct tm x; volatile __int64 t = 32535216000LL; int r;
    r = p_gm64(&x, (const __int64 *)&t); CHECK_EQ_I(r, 0); CHECK_EQ_I(x.tm_year, 3001 - 1900);
    t = 32536850399LL; r = p_gm64(&x, (const __int64 *)&t); CHECK_EQ_I(r, 0);
}
TC_CASE(gmtime32_negative_einval)
{
    struct tm x; volatile __int32 t = -86400; int r;
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
    TC_RUN(null_tm_einval); TC_RUN(null_time_einval); TC_RUN(negative_time_einval); TC_RUN(minus_one_accepted); TC_RUN(beyond_year_3000_einval); TC_RUN(year_3000_limit_is_loose); TC_RUN(gmtime32_negative_einval);
    return tc_end();
}
