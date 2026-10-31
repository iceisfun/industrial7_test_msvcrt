/* TC
 * id:    nfx_neg_atexit_registered_after_exit_started
 * area:  negative_fatal
 * kind:  negative
 * funcs: _crt_atexit exit
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.22.4.4 / 7.22.4.3 (registration while exit processing is unspecified); MS docs atexit, _crt_atexit (ucrtbase exports _crt_atexit; atexit is a static wrapper over it)
 * note:  Probes: calling _crt_atexit (atexit) from inside a running exit handler is accepted (returns 0) and the newly registered handler
 *        is run after the current one, giving the order "h1h2" in the marker file; process exits with code 0.
 */
#include "tc.h"
#include <stdlib.h>

int __cdecl _crt_atexit(void (__cdecl *)(void));

#define MK "tc_nfx_neg_atexit_registered_after_exit_started_m.tmp"
static void put(char c)
{
    HANDLE f = CreateFileA(MK, FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD n;
    if (f != INVALID_HANDLE_VALUE) { WriteFile(f, &c, 1, &n, NULL); CloseHandle(f); }
}
static void __cdecl h2(void) { put('2'); }
static void __cdecl h1(void) { put('1'); if (_crt_atexit(h2) != 0) put('E'); }

TC_CASE(nested_registration_runs_after)
{
    DWORD code = 99, n = 0; int ran; char buf[8] = {0};
    HANDLE f;
    DeleteFileA(MK);
    ran = tc_spawn_self("nest", &code, 20000);
    f = CreateFileA(MK, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (f != INVALID_HANDLE_VALUE) { ReadFile(f, buf, 7, &n, NULL); CloseHandle(f); }
    DeleteFileA(MK);
    CHECK(ran);
    CHECK_EQ_U(code, 0);
    CHECK_STR(buf, "12");
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { if (_crt_atexit(h1) != 0) return 1; exit(0); }
    TC_RUN(nested_registration_runs_after);
    return tc_end();
}
