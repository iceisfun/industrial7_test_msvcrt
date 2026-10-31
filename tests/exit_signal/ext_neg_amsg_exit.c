/* TC
 * id:    ext_neg_amsg_exit
 * area:  exit_signal
 * kind:  negative
 * procs: _amsg_exit
 * crt:   win7
 * tier:  p2
 * conf:  probe
 * ref:   internal export of msvcrt.dll / msvcr80-120 (not in ucrtbase); writes "R60xx" runtime error banner then exits
 * note:  Probes that _amsg_exit(25) terminates the process with exit code 255 (__crtExitProcess(255)). Error mode is set to
 *        stderr in the child so no message box can appear.
 */
#include "tc.h"
#include <stdlib.h>

TC_DECL_PROC(void, _amsg_exit, (int));

TC_CASE(child_exits_255)
{
    DWORD code = 0; int ran;
    if (!tc_p__amsg_exit) TC_SKIP("_amsg_exit not exported");
    ran = tc_spawn_self("amsg", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 255);
}
int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(_amsg_exit);
    if ((c = tc_child_name()) != NULL) {
        unsigned int (__cdecl *sem)(int) = (unsigned int (__cdecl *)(int))tc_crt_proc("_set_error_mode");
        SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
        if (sem) sem(1);
        if (tc_p__amsg_exit) tc_p__amsg_exit(25);
        return 77;
    }
    TC_RUN(child_exits_255);
    return tc_end();
}
