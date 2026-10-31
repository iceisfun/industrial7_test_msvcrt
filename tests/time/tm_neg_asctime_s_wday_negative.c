/* TC
 * id:    tm_neg_asctime_s_wday_negative
 * area:  time
 * kind:  negative
 * funcs:
 * procs: asctime_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs asctime_s: tm_wday 0-6, tm_sec 0-59, tm_mday 1-31, tm_mon 0-11, tm_hour 0-23; EINVAL on violation
 * note:  msvcrt.dll excluded: _s functions / handler support not assured there. tm_yday is deliberately not tested.
 *        Buffer contents after failure are not asserted here (see tm_neg_probe_asctime_s_fail_buf_empty).
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

TC_DECL_PROC(int, asctime_s, (char *, size_t, const struct tm *));
static struct tm t;
static void good(void) { t.tm_year = 117; t.tm_mon = 9; t.tm_mday = 23; t.tm_hour = 14; t.tm_min = 5; t.tm_sec = 9; t.tm_wday = 1; t.tm_yday = 0; t.tm_isdst = 0; }
static void bad(void)
{
    char b[26]; int r;
    tc_iph_count = 0; r = tc_p_asctime_s(b, 26, &t);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wday_neg) { good(); t.tm_wday = -1; bad(); }
TC_CASE(sec_neg)  { good(); t.tm_sec = -1; bad(); }
TC_CASE(mday_32)  { good(); t.tm_mday = 32; bad(); }
TC_CASE(mon_neg)  { good(); t.tm_mon = -1; bad(); }
TC_CASE(hour_neg) { good(); t.tm_hour = -1; bad(); }

int main(void)
{
    tc_begin(); tc_iph_install(); TC_LOAD_PROC(asctime_s);
    CHECK(tc_p_asctime_s); if (!tc_p_asctime_s) return tc_end();
    TC_RUN(wday_neg); TC_RUN(sec_neg); TC_RUN(mday_32); TC_RUN(mon_neg); TC_RUN(hour_neg);
    return tc_end();
}
