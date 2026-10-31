/* TC
 * id:    ucr_neg_purecall_exact_code_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: _purecall
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _purecall, abort
 * note:  PROBE: exit code of default _purecall in ucrtbase. Expected 3 (abort() with message box/Watson suppressed by tc_begin via _set_abort_behavior). Could instead be 0xC0000409.
 */
#include "tc.h"

TC_CASE(code)
{
    DWORD code = 0;
    CHECK(tc_spawn_self("pc", &code, 20000));
    CHECK_EQ_U(code, 3);   /* msvcrt.dll/old CRTs differ (255); crt: is win10 only */
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        void (__cdecl *pc)(void) = (void (__cdecl *)(void))tc_crt_proc("_purecall");
        if (!pc) ExitProcess(90);
        pc();
        return 77;
    }
    TC_RUN(code);
    return tc_end();
}
