/* TC
 * id:    dat_neg_get_tzname_fatal
 * area:  data_exports
 * kind:  negative
 * funcs: _get_tzname
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "Parameter validation": default invalid parameter handler terminates the process
 * note:  Probes the exact termination status (expected fast-fail 0xC0000409) when _get_tzname gets index 5 with no handler installed.
 */
#include "tc.h"
#include <time.h>

TC_CASE(child_terminates_fastfail)
{
    DWORD code = 0; int ran = tc_spawn_self("badidx", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { size_t r; char b[8]; volatile int idx = 5; _get_tzname(&r, b, sizeof b, idx); return 77; }
    TC_RUN(child_terminates_fastfail);
    return tc_end();
}
