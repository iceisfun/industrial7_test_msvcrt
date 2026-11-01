/* TC
 * id:    fpe_neg_controlfp_s_default_handler_child
 * area:  fpenv
 * funcs: _controlfp_s
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs Parameter validation; _controlfp_s (invalid mask -> invalid parameter handler)
 * note:  F (single class; the harness fix is the primary cause): mingw-w64 startup installs its own silent invalid-parameter handler, so the CRT default (fatal) path was never exercised; the child now restores the default with _set_invalid_parameter_handler(NULL).
 *        Consequence of the same fix, observed: baseline used new=0,mask=0x4000, which Windows accepts (rc 0, no handler: invalid bits are only checked in newControl&mask); observed on Win10 19045: _controlfp_s(&c,0x4000,0x4000) returns EINVAL(22) and calls the invalid parameter handler. Child now passes newControl=mask=0x4000.
 *        Fatal path in a child. Asserts abnormal termination only: exit code not 0 and not 77 (77 = call returned). Exact status is probed in fpe_neg_controlfp_s_default_handler_status_probe; Win7-era CRTs in fpe_neg_controlfp_s_default_handler_win7_probe.
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
    if (tc_child_name()) { unsigned c = 0; volatile unsigned m = 0x00004000; void (__cdecl *rst)(void *) = (void (__cdecl *)(void *))tc_crt_proc("_set_invalid_parameter_handler"); if (rst) rst(NULL); /* F: mingw crt startup installs a silent handler; NULL restores the CRT default */ _controlfp_s(&c, m, m); return 77; }
    TC_RUN(child_terminates); TC_RUN(parent_survives_valid_call);
    return tc_end();
}
