/* TC
 * id:    tm_strftime_probe_hash_long_date
 * area:  time
 * funcs: strftime
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs strftime ("#" flag: %#c / %#x use the long date format)
 * note:  PROBES the exact C-locale long date text "dddd, MMMM dd, yyyy" (zero-padded day), i.e. "Sunday, March 05, 2017".
 */
#include "tc.h"
#include <time.h>

static const char *fmt(const char *f)
{
    static char buf[96]; volatile const char *vf = f; struct tm t; char *p = (char *)&t; size_t i;
    for (i = 0; i < sizeof t; i++) p[i] = 0;
    t.tm_year = 117; t.tm_mon = 2; t.tm_mday = 5; t.tm_hour = 7; t.tm_min = 8; t.tm_sec = 9; t.tm_wday = 0; t.tm_yday = 63;
    if (strftime(buf, sizeof buf, (const char *)vf, &t) == 0) return "(zero)";
    return buf;
}
TC_CASE(long_x) { CHECK_STR(fmt("%#x"), "Sunday, March 05, 2017"); }
TC_CASE(long_c) { CHECK_STR(fmt("%#c"), "Sunday, March 05, 2017 07:08:09"); }
int main(void) { tc_begin(); TC_RUN(long_x); TC_RUN(long_c); return tc_end(); }
