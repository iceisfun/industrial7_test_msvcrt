/* TC
 * id:    sec_gmtime32_s_basic
 * area:  secure_crt
 * funcs:
 * procs: _gmtime32_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _gmtime32_s: 32-bit time_t, range 1970-01-01 through 2038-01-18 23:59:59 UTC is guaranteed; INT32_MAX is 2038-01-19 03:14:07
 */
#include "tc.h"
#include <time.h>

typedef int (__cdecl *gmtime32_s_t)(struct tm *, const __int32 *);
static gmtime32_s_t p_gm;

static void chk(__int32 t, int y, int mo, int d, int h, int mi, int s, int wd, int yd)
{
    struct tm tmv; volatile __int32 vt = t; int r;
    r = p_gm(&tmv, (const __int32 *)&vt);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(tmv.tm_year, y - 1900); CHECK_EQ_I(tmv.tm_mon, mo - 1); CHECK_EQ_I(tmv.tm_mday, d);
    CHECK_EQ_I(tmv.tm_hour, h); CHECK_EQ_I(tmv.tm_min, mi); CHECK_EQ_I(tmv.tm_sec, s);
    CHECK_EQ_I(tmv.tm_wday, wd); CHECK_EQ_I(tmv.tm_yday, yd); CHECK_EQ_I(tmv.tm_isdst, 0);
}
TC_CASE(epoch) { chk(0, 1970, 1, 1, 0, 0, 0, 4, 0); }
TC_CASE(int32_max_2038_01_19_03_14_07_tue) { chk(2147483647, 2038, 1, 19, 3, 14, 7, 2, 18); }
TC_CASE(t1_000_000_000_2001_09_09_01_46_40) { chk(1000000000, 2001, 9, 9, 1, 46, 40, 0, 251); }
int main(void)
{
    tc_begin();
    p_gm = (gmtime32_s_t)tc_crt_proc("_gmtime32_s");
    CHECK(p_gm); if (!p_gm) return tc_end();
    TC_RUN(epoch); TC_RUN(int32_max_2038_01_19_03_14_07_tue); TC_RUN(t1_000_000_000_2001_09_09_01_46_40);
    return tc_end();
}
