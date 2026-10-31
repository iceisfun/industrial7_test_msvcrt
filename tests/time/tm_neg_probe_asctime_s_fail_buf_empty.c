/* TC
 * id:    tm_neg_probe_asctime_s_fail_buf_empty
 * area:  time
 * kind:  negative
 * funcs:
 * procs: asctime_s
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs asctime_s
 * note:  Probes that after asctime_s rejects tm_mon=12 with EINVAL, a valid 26-byte buffer is left as an empty string
 *        (buf[0]==0). Not clearly documented, so isolated from the documented return/handler checks.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

TC_DECL_PROC(int, asctime_s, (char *, size_t, const struct tm *));

TC_CASE(mon12_buf)
{
    char b[26]; int i; struct tm t;
    for (i = 0; i < 26; i++) b[i] = 'x';
    t.tm_year = 117; t.tm_mon = 12; t.tm_mday = 23; t.tm_hour = 14; t.tm_min = 5; t.tm_sec = 9; t.tm_wday = 1; t.tm_yday = 0; t.tm_isdst = 0;
    CHECK_EQ_I(tc_p_asctime_s(b, 26, &t), EINVAL);
    CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin(); tc_iph_install(); TC_LOAD_PROC(asctime_s);
    CHECK(tc_p_asctime_s); if (!tc_p_asctime_s) return tc_end();
    TC_RUN(mon12_buf);
    return tc_end();
}
