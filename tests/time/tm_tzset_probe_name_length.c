/* TC
 * id:    tm_tzset_probe_name_length
 * area:  time
 * funcs:
 * procs: _get_tzname
 * crt:   >=90
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _tzset
 * note:  PROBES how many characters of a zone name are kept: believed full name ("ABCDE") on ucrtbase,
 *        truncated to 3 characters ("ABC") on the older CRTs. Nothing is documented.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

TC_DECL_PROC(int, _get_tzname, (size_t *, char *, size_t, int));

TC_CASE(long_std_name)
{
    char buf[32]; size_t sz = 0; int r;
    if (!tc_p__get_tzname) TC_SKIP("_get_tzname missing");
    _putenv("TZ=ABCDE5"); _tzset();
    buf[0] = 0;
    r = tc_p__get_tzname(&sz, buf, sizeof buf, 0);
    CHECK_EQ_I(r, 0);
#ifdef TC_UCRT
    CHECK_STR(buf, "ABCDE");
#else
    CHECK_STR(buf, "ABC");
#endif
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_get_tzname);
    TC_RUN(long_std_name);
    _putenv("TZ="); _tzset();
    return tc_end();
}
