/* TC
 * id:    tm_neg_asctime_probe_year_max
 * area:  time
 * funcs: asctime
 * procs: asctime_s
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs asctime_s
 * note:  PROBE: tm_year 8100 (year 10000, would need a 5-digit year) is rejected: asctime returns NULL
 *        with EINVAL, asctime_s returns EINVAL, handler invoked.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
TC_DECL_PROC(int, asctime_s, (char *, size_t, const struct tm *));
static struct tm t;
static void mk(void)
{ t.tm_year = 8100; t.tm_mon = 0; t.tm_mday = 1; t.tm_hour = 0; t.tm_min = 0; t.tm_sec = 0; t.tm_wday = 0; t.tm_yday = 0; t.tm_isdst = 0; }
TC_CASE(narrow) { char *r; mk(); errno = 0; tc_iph_count = 0; r = asctime(&t); CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(secure) { char b[26]; int r; mk(); tc_iph_count = 0; r = tc_p_asctime_s(b, 26, &t); CHECK_EQ_I(r, EINVAL); CHECK(tc_iph_count >= 1); }
int main(void) { tc_begin(); tc_iph_install(); TC_LOAD_PROC(asctime_s); TC_RUN(narrow); if (tc_p_asctime_s) TC_RUN(secure); return tc_end(); }
