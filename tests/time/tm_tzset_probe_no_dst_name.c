/* TC
 * id:    tm_tzset_probe_no_dst_name
 * area:  time
 * funcs:
 * procs: _get_tzname
 * crt:   >=90
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _tzset
 * note:  PROBES that tzname[1] is the empty string when TZ names no DST zone (TZ=EST5). Not documented.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

TC_DECL_PROC(int, _get_tzname, (size_t *, char *, size_t, int));

TC_CASE(dst_name_empty)
{
    char buf[32]; size_t sz = 99; int r;
    if (!tc_p__get_tzname) TC_SKIP("_get_tzname missing");
    _putenv("TZ=EST5"); _tzset();
    buf[0] = 'x'; buf[1] = 0;
    r = tc_p__get_tzname(&sz, buf, sizeof buf, 1);
    CHECK_EQ_I(r, 0);
    CHECK_STR(buf, "");
    CHECK_EQ_U(sz, 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_get_tzname);
    TC_RUN(dst_name_empty);
    _putenv("TZ="); _tzset();
    return tc_end();
}
