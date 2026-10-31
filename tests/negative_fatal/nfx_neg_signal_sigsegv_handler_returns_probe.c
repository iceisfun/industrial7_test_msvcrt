/* TC
 * id:    nfx_neg_signal_sigsegv_handler_returns_probe
 * area:  negative_fatal
 * kind:  negative
 * funcs: signal
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs signal (handler for SIGSEGV is reset to SIG_DFL before being called); C11 7.14.1.1
 * note:  Probes: a SIGSEGV handler that returns after a hardware fault lets the faulting instruction re-execute; the handler had been reset
 *        to SIG_DFL, so the second fault is unhandled and the process dies with 0xC0000005. The handler is called at most once.
 *        Toolchain note: exes are built with mingw-w64, whose startup does the exception-to-signal translation.
 */
#include "tc.h"
#include <signal.h>

#define MK "tc_nfx_neg_signal_sigsegv_handler_returns_probe_m.tmp"
static volatile char *volatile p;
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
TC_CASE(handler_returns_then_dies_av)
{
    DWORD code = 0; int ran; long n;
    DeleteFileA(MK);
    ran = tc_spawn_self("segv", &code, 20000);
    n = msize();
    DeleteFileA(MK);
    CHECK(ran);
    CHECK_EQ_U(code, 0xC0000005u);
    CHECK(n <= 1);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { signal(SIGSEGV, h); p = NULL; *p = 1; return 77; }
    TC_RUN(handler_returns_then_dies_av);
    return tc_end();
}
