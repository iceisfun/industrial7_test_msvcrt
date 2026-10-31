/* TC
 * id:    tm_asctime_probe_edges
 * area:  time
 * funcs: asctime
 * procs: asctime_s
 * crt:   >=90
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs asctime_s (tm_year range); C11 7.27.3.1
 * note:  PROBE (separate guesses, one case each): (1) tm_yday/tm_isdst are not validated or printed, even when
 *        out of range (yday=300 and yday=-5); (2) year 9999 (tm_year 8099) is accepted by asctime and
 *        asctime_s. The 8100 rejection is probed in tm_neg_asctime_probe_year_max.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
TC_DECL_PROC(int, asctime_s, (char *, size_t, const struct tm *));
static struct tm t;
static void mk(int y, int mo, int d, int h, int mi, int s, int wd)
{ t.tm_year = y; t.tm_mon = mo; t.tm_mday = d; t.tm_hour = h; t.tm_min = mi; t.tm_sec = s; t.tm_wday = wd; t.tm_yday = 0; t.tm_isdst = 0; }
TC_CASE(yday_big) { mk(117, 0, 15, 0, 0, 0, 0); t.tm_yday = 300; CHECK_STR(asctime(&t), "Sun Jan 15 00:00:00 2017\n"); }
TC_CASE(yday_neg_dst) { mk(117, 0, 15, 0, 0, 0, 0); t.tm_yday = -5; t.tm_isdst = 1; CHECK_STR(asctime(&t), "Sun Jan 15 00:00:00 2017\n"); }
TC_CASE(y9999) { mk(8099, 11, 31, 23, 59, 59, 5); CHECK_STR(asctime(&t), "Fri Dec 31 23:59:59 9999\n"); }
TC_CASE(y9999_s) { char b[26]; mk(8099, 11, 31, 23, 59, 59, 5); CHECK_EQ_I(tc_p_asctime_s(b, 26, &t), 0); CHECK_STR(b, "Fri Dec 31 23:59:59 9999\n"); }
int main(void) { tc_begin(); tc_iph_install(); TC_LOAD_PROC(asctime_s); TC_RUN(yday_big); TC_RUN(yday_neg_dst); TC_RUN(y9999); if (tc_p_asctime_s) TC_RUN(y9999_s); return tc_end(); }
