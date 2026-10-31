/* TC
 * id:    tm_neg_strftime_probe_trailing_percent
 * area:  time
 * funcs: strftime
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strftime
 * note:  PROBES the behaviour of a format ending in a lone '%'. Most likely rc 0, errno EINVAL, handler called once.
 */
#include "tc.h"
#include <time.h>
#include <wchar.h>
#include <errno.h>
#include <stdlib.h>

static void mk(struct tm *t, int Y, int Mo, int D, int h, int m, int s, int wd, int yd, int dst)
{
    t->tm_year = Y - 1900; t->tm_mon = Mo - 1; t->tm_mday = D; t->tm_hour = h; t->tm_min = m; t->tm_sec = s;
    t->tm_wday = wd; t->tm_yday = yd; t->tm_isdst = dst;
}

TC_CASE(trailing) { struct tm t; char b[32]; size_t r; mk(&t, 2020, 6, 15, 12, 30, 30, 1, 166, 0);
    errno = 0; tc_iph_count = 0; r = strftime(b, sizeof b, "abc%", &t);
    CHECK_EQ_U(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(trailing); return tc_end(); }
