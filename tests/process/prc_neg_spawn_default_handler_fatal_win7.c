/* TC
 * id:    prc_neg_spawn_default_handler_fatal_win7
 * area:  process
 * funcs: _spawnl
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs "Parameter validation": default handler terminates the process
 * note:  Exact status (0xC0000417 or 0xC0000409) deliberately not asserted; compare with the result log on real Windows.
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
    if (tc_child_name() != NULL) { _spawnl(_P_WAIT, NULL, "x", NULL); return 77; }
    TC_RUN(child_terminates);
    return tc_end();
}
