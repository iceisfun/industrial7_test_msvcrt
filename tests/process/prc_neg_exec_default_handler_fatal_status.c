/* TC
 * id:    prc_neg_exec_default_handler_fatal_status
 * area:  process
 * funcs: _execl
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs "Parameter validation"
 * note:  Observed on Win10 22H2 19045 ucrtbase: with the CRT's default invalid-parameter handler restored in the child
 *        (tc_iph_default(); the mingw startup installs a no-op handler that made the child just return 77) _execl(NULL, NULL)
 *        ends the process with STATUS_STACK_BUFFER_OVERRUN 0xC0000409 (fast-fail). Stable over repeated runs. Fatal path runs
 *        in a child process.
 */
#include "tc.h"
#include <process.h>

TC_CASE(child_status)
{
    DWORD code = 0; CHECK(tc_spawn_self("execnull", &code, 20000));
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}

int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) { const char *volatile p = NULL; tc_iph_default(); _execl(p, (char *)NULL); return 77; }
    TC_RUN(child_status);
    return tc_end();
}
