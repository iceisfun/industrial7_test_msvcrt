/* TC
 * id:    tm_neg_wasctime_s_fields
 * area:  time
 * kind:  negative
 * funcs:
 * procs: _wasctime_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs asctime_s, _wasctime_s (tm_year >= 0, mon 0..11, mday 1..31, hour 0..23, min 0..59, wday 0..6; out-of-range fields give EINVAL)
 * note:  msvcrt.dll excluded: invalid-parameter handler support there is not assured. The empty-string-on-failure convention
 *        is probed in tm_neg_probe_w_secure_buf_reset. boundary_ok uses year 8099 (tm_year 6199), well inside any 4-digit limit;
 *        the true upper limit (tm_year 8099 = year 9999) is probed in tm_neg_asctime_probe_year_max.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s, int wd)
{
    t->tm_year = y; t->tm_mon = mo; t->tm_mday = d; t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_wday = wd; t->tm_yday = 0; t->tm_isdst = 0;
}
TC_DECL_PROC(int, _wasctime_s, (wchar_t *, size_t, const struct tm *));
static struct tm t;
static void bad(void) { wchar_t b[26]; int r; b[0] = L'x'; tc_iph_count = 0; r = tc_p__wasctime_s(b, 26, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
#define GOOD mk(&t, 117, 9, 23, 14, 5, 9, 1)
TC_CASE(year) { GOOD; t.tm_year = -1; bad(); }
TC_CASE(mon) { GOOD; t.tm_mon = 12; bad(); GOOD; t.tm_mon = -1; bad(); }
TC_CASE(mday) { GOOD; t.tm_mday = 0; bad(); GOOD; t.tm_mday = 32; bad(); }
TC_CASE(hour) { GOOD; t.tm_hour = 24; bad(); }
TC_CASE(min) { GOOD; t.tm_min = 60; bad(); }
TC_CASE(wday) { GOOD; t.tm_wday = 7; bad(); GOOD; t.tm_wday = -1; bad(); }
TC_CASE(boundary_ok) { wchar_t b[26]; int r; mk(&t, 6199, 11, 31, 23, 59, 59, 0); tc_iph_count = 0; r = tc_p__wasctime_s(b, 26, &t); CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_WSTR(b, L"Sun Dec 31 23:59:59 8099\n"); }
int main(void)
{
    tc_begin(); tc_iph_install(); TC_LOAD_PROC(_wasctime_s); CHECK(tc_p__wasctime_s); if (!tc_p__wasctime_s) return tc_end();
    TC_RUN(year); TC_RUN(mon); TC_RUN(mday); TC_RUN(hour); TC_RUN(min); TC_RUN(wday); TC_RUN(boundary_ok); return tc_end();
}
