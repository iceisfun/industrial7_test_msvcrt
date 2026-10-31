/* TC
 * id:    ext_neg_unhandled_av_status_no_signal_handler
 * area:  exit_signal
 * funcs: signal
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   Win32 SetErrorMode / unhandled exception exit status; MS docs signal (SIGSEGV)
 * note:  Mostly an OS behaviour (exe is mingw-built, so no MSVC startup exception filter); exit code = exception code is widely observed, not CRT-documented.
 *        Child writes through NULL with no SIGSEGV handler and error dialogs suppressed: process exit code is the
 *        exception code 0xC0000005.
 */
#include "tc.h"
#include <signal.h>

TC_CASE(av_status)
{
    DWORD c = 0;
    CHECK(tc_spawn_self("av", &c, 20000));
    CHECK_EQ_U(c, 0xC0000005u);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_FAILCRITICALERRORS);
        if (signal(SIGSEGV, SIG_DFL) == SIG_ERR) return 78;
        *(volatile int *)0 = 1;
        return 77;
    }
    TC_RUN(av_status);
    return tc_end();
}
