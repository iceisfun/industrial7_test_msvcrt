/* TC
 * id:    sec_gmtime64_s_basic
 * area:  secure_crt
 * funcs:
 * procs: _gmtime64_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _gmtime64_s: valid range is 1970-01-01 00:00:00 UTC through 3000-12-31 23:59:59 UTC
 */
#include "tc.h"
#include <time.h>

typedef int (__cdecl *gmtime64_s_t)(struct tm *, const __int64 *);
static gmtime64_s_t p_gm;

static void chk(__int64 t, int y, int mo, int d, int h, int mi, int s, int wd, int yd)
{
    struct tm tmv; volatile __int64 vt = t; int r;
    tmv.tm_year = 12345;
    r = p_gm(&tmv, (const __int64 *)&vt);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(tmv.tm_year, y - 1900); CHECK_EQ_I(tmv.tm_mon, mo - 1); CHECK_EQ_I(tmv.tm_mday, d);
    CHECK_EQ_I(tmv.tm_hour, h); CHECK_EQ_I(tmv.tm_min, mi); CHECK_EQ_I(tmv.tm_sec, s);
    CHECK_EQ_I(tmv.tm_wday, wd); CHECK_EQ_I(tmv.tm_yday, yd);
    CHECK_EQ_I(tmv.tm_isdst, 0);
}
TC_CASE(epoch_1970_thu) { chk(0, 1970, 1, 1, 0, 0, 0, 4, 0); }
TC_CASE(t951782400_2000_02_29_tue) { chk(951782400LL, 2000, 2, 29, 0, 0, 0, 2, 59); }
TC_CASE(t86399_end_of_day1) { chk(86399, 1970, 1, 1, 23, 59, 59, 4, 0); }
TC_CASE(max_32535215999_year3000_dec31_yday364) { chk(32535215999LL, 3000, 12, 31, 23, 59, 59, 3, 364); }
TC_CASE(leap_yday_check)
{
    chk(951868800LL, 2000, 3, 1, 0, 0, 0, 3, 60);        /* 2000 is a leap year: Mar 1 = yday 60 */
    chk(978220800LL, 2000, 12, 31, 0, 0, 0, 0, 365);     /* Dec 31 2000 = yday 365, Sunday */
    chk(981000000LL - 981000000LL % 86400, 2001, 2, 1, 0, 0, 0, 4, 31);
}
int main(void)
{
    tc_begin();
    p_gm = (gmtime64_s_t)tc_crt_proc("_gmtime64_s");
    CHECK(p_gm); if (!p_gm) return tc_end();
    TC_RUN(epoch_1970_thu); TC_RUN(t951782400_2000_02_29_tue); TC_RUN(t86399_end_of_day1);
    TC_RUN(max_32535215999_year3000_dec31_yday364); TC_RUN(leap_yday_check);
    return tc_end();
}
