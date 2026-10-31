/* TC
 * id:    dat_neg_get_tz_null_fatal
 * area:  data_exports
 * funcs: _get_daylight _get_timezone _get_dstbias _get_tzname
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation: default invalid parameter handler terminates the process; _get_daylight etc. validate NULL
 * note:  Exact termination status is covered by *_status_probe tests; here only: abnormal, not returning (77).
 */
#include "tc.h"
#include <stdlib.h>
#include <time.h>

static void abnormal(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
TC_CASE(daylight)       { abnormal("daylight"); }
TC_CASE(timezone)       { abnormal("timezone"); }
TC_CASE(dstbias)        { abnormal("dstbias"); }
TC_CASE(tzname_retval)  { abnormal("tzname_retval"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        long *volatile lp = NULL; int *volatile ip = NULL; size_t *volatile sp = NULL; char b[16];
        if (c[0] == 'd' && c[1] == 'a') _get_daylight(ip);
        else if (c[0] == 't' && c[1] == 'i') _get_timezone(lp);
        else if (c[0] == 'd') _get_dstbias(lp);
        else _get_tzname(sp, b, sizeof b, 0);
        return 77;
    }
    TC_RUN(daylight); TC_RUN(timezone); TC_RUN(dstbias); TC_RUN(tzname_retval);
    return tc_end();
}
