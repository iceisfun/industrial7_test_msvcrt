/* TC
 * id:    fpe_neg_controlfp_s_default_handler_win7_probe
 * area:  fpenv
 * funcs: _controlfp_s
 * procs: 
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs Parameter validation; _controlfp_s (invalid mask -> invalid parameter handler)
 * note:  Probe: the default invalid-parameter handler of Win7-era CRTs (msvcr90..120) is fatal for _controlfp_s with an undefined mask bit (0x4000). Asserts abnormal termination only (exit code not 0 and not 77).
 */
#include "tc.h"
#include <float.h>

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("badmask", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
}
TC_CASE(parent_survives_valid_call)
{
    unsigned c = 0;
    CHECK_EQ_I(_controlfp_s(&c, 0, 0), 0);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { unsigned c = 0; volatile unsigned m = 0x00004000; _controlfp_s(&c, 0, m); return 77; }
    TC_RUN(child_terminates); TC_RUN(parent_survives_valid_call);
    return tc_end();
}
