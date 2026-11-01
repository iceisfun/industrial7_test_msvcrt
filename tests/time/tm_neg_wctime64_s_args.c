/* TC
 * id:    tm_neg_wctime64_s_args
 * area:  time
 * kind:  negative
 * funcs: _wctime64_s
 * crt:   >=90
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _wctime_s, _wctime32_s, _wctime64_s (buffer >= 26 wide chars; time before 1970 or after 3000-12-31 23:59:59 invalid)
 * note:  Win10 19045 ucrtbase: negative timer returns EINVAL/errno 22 WITHOUT calling the handler; the upper bound is 32536799999, so past_year_3000 uses 32536800000.
 */
#include "tc.h"

/* Observed limits of the real Windows 10 ucrtbase (build 19045): _MAX__TIME64_T = 32536799999 (3001-01-19 07:59:59 UTC) for local-time style
 * conversions (_localtime64, _ctime64*, _wctime64*, _mktime64, _mkgmtime64, _futime64); _gmtime64 accepts [-43200, 32536850399].
 * The MS docs say 3000-12-31 23:59:59 UTC / 1970-01-01; Windows is more permissive. Older CRTs keep the documented-era values (unverified). */
#if TC_CRT_VER >= 140
#define T64_LOCAL_BAD 32536800000LL
#define T64_GM_BAD    32536850400LL
#define T64_GM_NEG_BAD (-43201LL)
#else
#define T64_LOCAL_BAD 32535244800LL
#define T64_GM_BAD    32535244800LL
#define T64_GM_NEG_BAD (-1LL)
#endif
#include <time.h>
#include <errno.h>

static volatile __time64_t v_t0 = 0, v_neg = -1, v_big = T64_LOCAL_BAD;

TC_CASE(size_25)
{
    wchar_t buf[40]; __time64_t t = v_t0; errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(_wctime64_s(buf, 25, &t), EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(null_buffer)
{
    __time64_t t = v_t0; errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(_wctime64_s(NULL, 26, &t), EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(negative_timer)
{
    wchar_t buf[40]; __time64_t t = v_neg; errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(_wctime64_s(buf, 40, &t), EINVAL);
    CHECK_EQ_I(errno, EINVAL);
#if TC_CRT_VER >= 140
    CHECK_EQ_I(tc_iph_count, 0);   /* observed on Win10 ucrtbase: the range error does not call the handler */
#else
    CHECK(tc_iph_count >= 1);
#endif
}
TC_CASE(past_year_3000)
{
    wchar_t buf[40]; __time64_t t = v_big; errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(_wctime64_s(buf, 40, &t), EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    /* whether the invalid-parameter handler runs for the upper range check is not documented: see tm_neg_probe_wctime64_s_3000_iph */
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(size_25); TC_RUN(null_buffer); TC_RUN(negative_timer); TC_RUN(past_year_3000);
    return tc_end();
}
