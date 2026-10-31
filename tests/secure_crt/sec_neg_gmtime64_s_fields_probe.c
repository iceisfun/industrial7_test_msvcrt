/* TC
 * id:    sec_neg_gmtime64_s_fields_probe
 * area:  secure_crt
 * kind:  negative
 * funcs:
 * procs: _gmtime64_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _gmtime64_s
 * note:  Probes: after a range failure (time = -1) all nine struct tm fields are set to -1 (the secure time functions' documented
 *        habit for the error path); the return value EINVAL itself is covered by sec_neg_gmtime64_s_range.
 */
#include "tc.h"
#include <time.h>

typedef int (__cdecl *gm64_t)(struct tm *, const __int64 *);
static gm64_t p_gm64;

TC_CASE(fields_minus_one_after_failure)
{
    struct tm x; volatile __int64 t = -1; int r;
    x.tm_sec = x.tm_min = x.tm_hour = x.tm_mday = x.tm_mon = x.tm_year = x.tm_wday = x.tm_yday = x.tm_isdst = 7;
    r = p_gm64(&x, (const __int64 *)&t);
    CHECK(r != 0);
    CHECK_EQ_I(x.tm_sec, -1); CHECK_EQ_I(x.tm_min, -1); CHECK_EQ_I(x.tm_hour, -1); CHECK_EQ_I(x.tm_mday, -1); CHECK_EQ_I(x.tm_mon, -1);
    CHECK_EQ_I(x.tm_year, -1); CHECK_EQ_I(x.tm_wday, -1); CHECK_EQ_I(x.tm_yday, -1); CHECK_EQ_I(x.tm_isdst, -1);
}
int main(void)
{
    tc_begin();
    p_gm64 = (gm64_t)tc_crt_proc("_gmtime64_s");
    CHECK(p_gm64); if (!p_gm64) return tc_end();
    tc_iph_install();
    TC_RUN(fields_minus_one_after_failure);
    return tc_end();
}
