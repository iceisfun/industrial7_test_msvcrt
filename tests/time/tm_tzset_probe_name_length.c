/* TC
 * id:    tm_tzset_probe_name_length
 * area:  time
 * funcs:
 * procs: _get_tzname
 * crt:   >=90
 * tier:  p2
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _tzset
 * note:  PROBES how many characters of a zone name are kept: believed full name ("ABCDE") on ucrtbase,
 *        truncated to 3 characters ("ABC") on the older CRTs. Nothing is documented.
 *        OBSERVED (Win10 22H2 19045 oracle93[34])?#934 integration full run crt-triage-triage-r1 and the baseline): ucrtbase ALSO keeps 3 characters, _get_tzname(.., 0) gives "ABC" for
 *        TZ=ABCDE5 (consistent with MS docs _tzset: "tzn: three-letter time-zone name"). Class A: the earlier ">=140 -> ABCDE" gate
 *        was not observed (it never ran: TC_UCRT was undefined in the baseline build).
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
    CHECK_STR(buf, "ABC");   /* ucrtbase observed on Win10 22H2; older CRTs: original expectation (not run on Windows) */
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_get_tzname);
    TC_RUN(long_std_name);
    _putenv("TZ="); _tzset();
    return tc_end();
}
