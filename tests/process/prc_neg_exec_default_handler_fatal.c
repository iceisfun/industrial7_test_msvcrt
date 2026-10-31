/* TC
 * id:    prc_neg_exec_default_handler_fatal
 * area:  process
 * funcs: _execl
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs "Parameter validation", _exec: NULL cmdname is an invalid parameter; the default handler terminates the process
 * note:  Exact status is in prc_neg_exec_default_handler_fatal_status (probe).
 */
#include "tc.h"
#include <process.h>

TC_CASE(child_terminates)
{
    DWORD code = 0; CHECK(tc_spawn_self("execnull", &code, 20000));
    CHECK(code != 0);
    CHECK(code != 77);
}

int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) { const char *volatile p = NULL; _execl(p, (char *)NULL); return 77; }
    TC_RUN(child_terminates);
    return tc_end();
}
