/* TC
 * id:    prc_neg_spawn_default_handler_fatal_status
 * area:  process
 * funcs: _spawnl
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   see err_neg_default_handler_fatal
 * note:  Observed on Win10 22H2 19045 ucrtbase: with the CRT's default invalid-parameter handler restored in the child
 *        (tc_iph_default(); the mingw startup installs a no-op handler that made the child just return 77) _spawnl(_P_WAIT,
 *        NULL, ...) ends the process with STATUS_STACK_BUFFER_OVERRUN 0xC0000409 (fast-fail). Stable over repeated runs. Fatal
 *        path runs in a child process.
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
    if (tc_child_name() != NULL) { tc_iph_default(); _spawnl(_P_WAIT, NULL, "x", NULL); return 77; }
    TC_RUN(status_fastfail);
    return tc_end();
}
