/* TC
 * id:    nfx_neg_signal_sigterm_handler_reset_after_call
 * area:  negative_fatal
 * kind:  negative
 * funcs: signal raise
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs signal ("the value of func is set to SIG_DFL" before the handler runs); C11 7.14.1.1p3 (implementation resets to SIG_DFL or blocks)
 * note:  Probes: after raise(SIGTERM) calls a handler that does not re-register itself, the disposition is back to SIG_DFL, so a second
 *        raise(SIGTERM) terminates the process (nonzero, not 77) and the handler ran exactly once. If the disposition were NOT reset the
 *        handler would run twice and the child would survive (that outcome is also reported distinctly by exit 66).
 */
#include "tc.h"
#include <signal.h>

#define MK "tc_nfx_neg_signal_sigterm_handler_reset_after_call_m.tmp"
static void __cdecl h(int s)
{
    HANDLE f = CreateFileA(MK, FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD n;
    (void)s;
    if (f != INVALID_HANDLE_VALUE) { WriteFile(f, "H", 1, &n, NULL); CloseHandle(f); }
}
static long msize(void)
{
    HANDLE f = CreateFileA(MK, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD s;
    if (f == INVALID_HANDLE_VALUE) return -1;
    s = GetFileSize(f, NULL); CloseHandle(f); return (long)s;
}
TC_CASE(second_raise_terminates)
{
    DWORD code = 0; int ran; long n;
    DeleteFileA(MK);
    ran = tc_spawn_self("twice", &code, 20000);
    n = msize();
    DeleteFileA(MK);
    CHECK(ran);
    CHECK_EQ_I(n, 1);
    CHECK(code != 0);
    CHECK(code != 77);
    CHECK(code != 66);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        if (signal(SIGTERM, h) == SIG_ERR) return 1;
        if (raise(SIGTERM) != 0) return 2;
        raise(SIGTERM);
        return 66;
    }
    TC_RUN(second_raise_terminates);
    return tc_end();
}
