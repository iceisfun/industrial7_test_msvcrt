/* TC
 * id:    tm_tzset_offset_syntax
 * area:  time
 * funcs: _tzset _get_timezone _get_daylight
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _tzset: tzn[+|-]hh[:mm[:ss]][dzn]
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

static void chk(const char *env, long wantz, int wantd, int line)
{
    long tz = 12345; int dl = 12345;
    _putenv(env); _tzset(); _get_timezone(&tz); _get_daylight(&dl);
    if (tz != wantz) tc_fail_ints(__FILE__, line, env, "timezone", tz, wantz);
    if (wantd >= 0 && dl != wantd) tc_fail_ints(__FILE__, line, env, "daylight", dl, wantd);
}
TC_CASE(est5) { chk("TZ=EST5", 18000, 0, __LINE__); }
TC_CASE(jst) { chk("TZ=JST-9", -32400, 0, __LINE__); }
TC_CASE(minutes) { chk("TZ=XXX3:30", 12600, 0, __LINE__); }
TC_CASE(seconds) { chk("TZ=XXX5:30:15", 19815, 0, __LINE__); }
TC_CASE(neg_dst) { chk("TZ=CET-1CEST", -3600, 1, __LINE__); }
int main(void) { tc_begin(); TC_RUN(est5); TC_RUN(jst); TC_RUN(minutes); TC_RUN(seconds); TC_RUN(neg_dst); return tc_end(); }
