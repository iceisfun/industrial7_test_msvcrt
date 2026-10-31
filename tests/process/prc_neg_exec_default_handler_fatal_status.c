/* TC
 * id:    prc_neg_exec_default_handler_fatal_status
 * area:  process
 * funcs: _execl
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs "Parameter validation"
 * note:  PROBES: the ucrtbase default invalid-parameter handler ends the process with STATUS_STACK_BUFFER_OVERRUN
 *        (0xC0000409, __fastfail) when _execl(NULL, ...) is called.
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
    if (tc_child_name() != NULL) { const char *volatile p = NULL; _execl(p, (char *)NULL); return 77; }
    TC_RUN(child_status);
    return tc_end();
}
