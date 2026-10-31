/* TC
 * id:    tm_neg_wasctime_s_args
 * area:  time
 * kind:  negative
 * funcs:
 * procs: _wasctime_s
 * crt:   >=90 msvcrt
 * tier:  p1
 * conf:  documented
 * note:  Single-digit days are avoided here because day padding differs between CRT generations; see tm_ctime_probe_day_padding.
 * ref:   MS docs asctime_s, _wasctime_s (error conditions table: EINVAL)
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

TC_DECL_PROC(int, _wasctime_s, (wchar_t *, size_t, const struct tm *));
static struct tm good = { 0, 0, 0, 11, 0, 70, 0, 10, 0 };
static volatile int sink;

#define NEEDIPH() do { if (!tc_p__wasctime_s) TC_SKIP("not exported"); if (!tc_iph_install()) TC_SKIP("no invalid parameter handler"); tc_iph_count = 0; } while (0)

TC_CASE(good_26)
{
    wchar_t b[26]; struct tm t = good;
    if (!tc_p__wasctime_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__wasctime_s(b, 26, &t), 0);
    CHECK_WSTR(b, L"Sun Jan 11 00:00:00 1970\n");
}
TC_CASE(size_25)
{
    wchar_t b[32]; struct tm t = good; int r; NEEDIPH();
    r = tc_p__wasctime_s(b, 25, &t);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buf)
{
    struct tm t = good; int r; NEEDIPH();
    r = tc_p__wasctime_s(NULL, 26, &t);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_tm)
{
    wchar_t b[26]; int r; NEEDIPH();
    r = tc_p__wasctime_s(b, 26, NULL);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(hour_24)
{
    wchar_t b[26]; struct tm t = good; int r; NEEDIPH();
    sink = 24; t.tm_hour = sink;
    r = tc_p__wasctime_s(b, 26, &t);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_wasctime_s);
    TC_RUN(good_26); TC_RUN(size_25); TC_RUN(null_buf); TC_RUN(null_tm); TC_RUN(hour_24);
    return tc_end();
}
