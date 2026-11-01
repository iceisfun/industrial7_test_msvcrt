/* TC
 * id:    prc_neg_spawn_default_handler_fatal
 * area:  process
 * funcs: _spawnl
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs "Parameter validation", _spawnl: NULL cmdname is an invalid parameter; default handler terminates
 * note:  Documented part: child terminates abnormally. The exact status is in prc_neg_spawn_default_handler_fatal_status (probe).
 */
#include "tc.h"
#include <process.h>

TC_CASE(child_terminates)
{
    DWORD code = 0; CHECK(tc_spawn_self("spawnnull", &code, 20000));
    CHECK(code != 0);
    CHECK(code != 77);
}

int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) { tc_iph_default(); _spawnl(_P_WAIT, NULL, "x", NULL); return 77; }
    TC_RUN(child_terminates);
    return tc_end();
}
