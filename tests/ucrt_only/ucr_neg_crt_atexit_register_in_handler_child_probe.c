/* TC
 * id:    ucr_neg_crt_atexit_register_in_handler_child_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs: _crt_atexit
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.22.4.2 (atexit during exit processing is unspecified); _crt_atexit is an undocumented UCRT-internal registration
 * note:  PROBES that _crt_atexit called from inside an exit-time handler does not crash and returns 0. Whether the late handler runs
 *        is NOT asserted. Child writes the return value digit to a marker file; parent expects first marker byte '0'.
 */
#include "tc.h"
#include <corecrt_startup.h>

static const char *marker = "tc_ucr_neg_crt_atexit_register_in_handler_child_probe.tmp";

static void put(char ch)
{
    HANDLE h = CreateFileA(marker, FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD n;
    if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, &ch, 1, &n, NULL); CloseHandle(h);
}
static void late(void) { put('L'); }
static void outer(void)
{
    volatile int r = _crt_atexit(late);
    put(r == 0 ? '0' : '1');
}

TC_CASE(register_returns_zero)
{
    DWORD code = 99; HANDLE h; char buf[8]; DWORD n = 0;
    DeleteFileA(marker);
    CHECK(tc_spawn_self("nested", &code, 20000));
    CHECK_EQ_U(code, 0);
    h = CreateFileA(marker, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { ReadFile(h, buf, 7, &n, NULL); CloseHandle(h); }
    buf[n] = 0;
    CHECK(n >= 1 && buf[0] == '0');
    DeleteFileA(marker);
}

int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        volatile int r = _crt_atexit(outer);
        return r ? 78 : 0;
    }
    TC_RUN(register_returns_zero);
    return tc_end();
}
