/* TC
 * id:    tm_neg_probe_get_tzname_small_buffer
 * area:  time
 * funcs:
 * procs: _get_tzname
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _get_tzname
 * note:  PROBES the result for a non-NULL buffer smaller than the name: ERANGE returned and the required size (4 for
 *        "PST") stored in *returnValue. The docs error table does not clearly list this case.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(int, _get_tzname, (size_t *, char *, size_t, int));

TC_CASE(buffer_too_small_return)
{
    char buf[2] = {'x', 'x'}; size_t sz = 0; int r;
    if (!tc_p__get_tzname) TC_SKIP("missing");
    _putenv("TZ=PST8PDT"); _tzset();
    r = tc_p__get_tzname(&sz, buf, 2, 0);
    CHECK_EQ_I(r, ERANGE);
}
TC_CASE(buffer_too_small_size)
{
    char buf[2] = {'x', 'x'}; size_t sz = 0;
    if (!tc_p__get_tzname) TC_SKIP("missing");
    _putenv("TZ=PST8PDT"); _tzset();
    tc_p__get_tzname(&sz, buf, 2, 0);
    CHECK_EQ_U(sz, 4);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_LOAD_PROC(_get_tzname);
    TC_RUN(buffer_too_small_return); TC_RUN(buffer_too_small_size);
    _putenv("TZ="); _tzset();
    return tc_end();
}
