/* TC
 * id:    prc_neg_spawn_default_handler_fatal_status
 * area:  process
 * funcs: _spawnl
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   see err_neg_default_handler_fatal
 * note:  Probes that the UCRT default invalid-parameter path ends in fast-fail status 0xC0000409.
 */
#include "tc.h"
#include <process.h>

TC_CASE(status_fastfail)
{
    DWORD code = 0; CHECK(tc_spawn_self("spawnnull", &code, 20000));
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}

int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) { _spawnl(_P_WAIT, NULL, "x", NULL); return 77; }
    TC_RUN(status_fastfail);
    return tc_end();
}
