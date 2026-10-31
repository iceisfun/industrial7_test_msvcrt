/* TC
 * id:    fpe_neg_controlfp_s_default_handler_status_probe
 * area:  fpenv
 * funcs: _controlfp_s
 * procs: 
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs Parameter validation
 * note:  Probes the exact termination status of the UCRT default handler path from _controlfp_s with an invalid mask: expected fast-fail 0xC0000409.
 */
#include "tc.h"
#include <float.h>

TC_CASE(status)
{
    DWORD code = 0;
    CHECK(tc_spawn_self("badmask", &code, 20000));
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { unsigned c = 0; volatile unsigned m = 0x00004000; _controlfp_s(&c, 0, m); return 77; }
    TC_RUN(status);
    return tc_end();
}
