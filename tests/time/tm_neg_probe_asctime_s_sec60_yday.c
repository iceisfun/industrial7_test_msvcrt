/* TC
 * id:    tm_neg_probe_asctime_s_sec60_yday
 * area:  time
 * kind:  negative
 * funcs:
 * procs: asctime_s
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs asctime_s (tm_sec documented 0-59)
 * note:  Probes (a) tm_sec 60 and 61 (leap seconds) are rejected with EINVAL, (b) tm_yday 999 and tm_isdst 5 are not
 *        validated and succeed. Each is an independent case.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

TC_DECL_PROC(int, asctime_s, (char *, size_t, const struct tm *));
static struct tm t;
static void good(void) { t.tm_year = 117; t.tm_mon = 9; t.tm_mday = 23; t.tm_hour = 14; t.tm_min = 5; t.tm_sec = 9; t.tm_wday = 1; t.tm_yday = 0; t.tm_isdst = 0; }

TC_CASE(sec60) { char b[26]; good(); t.tm_sec = 60; tc_iph_count = 0; CHECK_EQ_I(tc_p_asctime_s(b, 26, &t), EINVAL); }
TC_CASE(sec61) { char b[26]; good(); t.tm_sec = 61; tc_iph_count = 0; CHECK_EQ_I(tc_p_asctime_s(b, 26, &t), EINVAL); }
TC_CASE(yday999) { char b[26]; good(); t.tm_yday = 999; CHECK_EQ_I(tc_p_asctime_s(b, 26, &t), 0); CHECK_STR(b, "Mon Oct 23 14:05:09 2017\n"); }
TC_CASE(isdst5)  { char b[26]; good(); t.tm_isdst = 5; CHECK_EQ_I(tc_p_asctime_s(b, 26, &t), 0); CHECK_STR(b, "Mon Oct 23 14:05:09 2017\n"); }

int main(void)
{
    tc_begin(); tc_iph_install(); TC_LOAD_PROC(asctime_s);
    CHECK(tc_p_asctime_s); if (!tc_p_asctime_s) return tc_end();
    TC_RUN(sec60); TC_RUN(sec61); TC_RUN(yday999); TC_RUN(isdst5);
    return tc_end();
}
