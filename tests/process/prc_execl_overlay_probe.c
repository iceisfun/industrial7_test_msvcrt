/* TC
 * id:    prc_execl_overlay_probe
 * area:  process
 * funcs: _execl
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _exec, _wexec functions
 * note:  Probes the known Windows quirk that _exec* creates a new process and terminates the caller with status 0
 *        without waiting for it. Parent observes the exit code of the first child only.
 */
#include "tc.h"
#include <process.h>

TC_CASE(caller_exits_zero)
{
    DWORD code = 99; int ran = tc_spawn_self("exec1", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 0);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (tc_strequ(c, "exec1")) {
            char exe[MAX_PATH]; GetModuleFileNameA(NULL, exe, MAX_PATH);
            _execl(exe, exe, "--tc-child=exec2", NULL);
            return 77;   /* _execl returned: failure */
        }
        return 0;        /* exec2 */
    }
    TC_RUN(caller_exits_zero);
    return tc_end();
}
