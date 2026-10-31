/* TC
 * id:    scn_neg_child_legacy_default_probe
 * area:  scanf
 * kind:  negative
 * funcs: sscanf
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs "Parameter validation" (CRT 8.0+): default handler invokes Watson / terminates
 * note:  PROBES: with no handler installed, sscanf(NULL,"%d",&x) terminates the child abnormally on pre-UCRT CRTs. The exact
 *        exit status varies (0xC0000417, 0xC0000409, 3, ...) so only "not 0 and not 77" is asserted; the code is the measurement.
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile vs;
TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("nullsscanf", &code, 20000);
    CHECK(ran); CHECK(code != 77); CHECK(code != 0);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { int x = 0; vs = NULL; sscanf(vs, "%d", &x); return 77; }
    TC_RUN(child_terminates);
    return tc_end();
}
