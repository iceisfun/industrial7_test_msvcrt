/* TC
 * id:    pfp_neg_fatal_old_n_default_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs "Parameter validation"; printf %n disabled by default (VS2005+)
 * note:  PROBE: sprintf("%n") with the default invalid-parameter handler terminates the process on 9.0-12.0 CRTs
 *        (Watson / crash). Only "not exit code 0 and not 77" is asserted; the exact status is not.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(probe_child_dies)
{
    DWORD code = 0; int ran = tc_spawn_self("pctn", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
}

int main(void)
{
    tc_begin();
    if (tc_child_name()) { char b[16]; volatile int n = 0; sprintf(b, "ab%n", (int *)&n); return 77; }
    TC_RUN(probe_child_dies);
    return tc_end();
}
