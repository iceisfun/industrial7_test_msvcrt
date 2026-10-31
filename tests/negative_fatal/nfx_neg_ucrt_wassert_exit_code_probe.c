/* TC
 * id:    nfx_neg_ucrt_wassert_exit_code_probe
 * area:  negative_fatal
 * kind:  negative
 * funcs: _wassert signal _set_error_mode
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs assert, _wassert, abort (exit code 3, raises SIGABRT)
 * note:  Probes: calling _wassert directly with error mode _OUT_TO_STDERR (abort message/Watson disabled by tc_begin) raises SIGABRT
 *        (handler runs first, here it records a marker file and returns) and then terminates the process with exit code 3.
 */
#include "tc.h"
#include <stdlib.h>
#include <signal.h>
#include <assert.h>

#define MK "tc_nfx_neg_ucrt_wassert_exit_code_probe_m.tmp"
static void __cdecl h(int s)
{
    HANDLE f = CreateFileA(MK, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD n;
    (void)s;
    if (f != INVALID_HANDLE_VALUE) { WriteFile(f, "S", 1, &n, NULL); CloseHandle(f); }
}
TC_CASE(wassert_sigabrt_then_exit3)
{
    DWORD code = 0, at; int ran;
    DeleteFileA(MK);
    ran = tc_spawn_self("wassert", &code, 20000);
    at = GetFileAttributesA(MK);
    DeleteFileA(MK);
    CHECK(ran);
    CHECK(at != INVALID_FILE_ATTRIBUTES);
    CHECK_EQ_U(code, 3);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        _set_error_mode(_OUT_TO_STDERR);
        signal(SIGABRT, h);
        _wassert(L"x", L"f.c", 7);
        return 77;
    }
    TC_RUN(wassert_sigabrt_then_exit3);
    return tc_end();
}
