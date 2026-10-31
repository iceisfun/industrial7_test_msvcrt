/* TC
 * id:    ucr_neg_crt_atexit_exit_in_handler_child
 * area:  ucrt_only
 * kind:  negative
 * funcs: _crt_atexit _exit
 * crt:   win10
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.22.4.5 / MS docs _exit (terminates without calling atexit handlers); _crt_atexit is an undocumented UCRT-internal registration
 * note:  PROBES that _exit(33) called from a handler during exit processing (after return from main) ends the process with code 33
 *        and that handlers registered earlier (which would run later, LIFO) are skipped. Child registers hA (writes marker 'A') then hB (calls _exit(33)).
 */
#include "tc.h"
#include <process.h>
#include <corecrt_startup.h>

static const char *marker = "tc_ucr_neg_crt_atexit_exit_in_handler_child.tmp";

static void put(char ch)
{
    HANDLE h = CreateFileA(marker, FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD n;
    if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, &ch, 1, &n, NULL); CloseHandle(h);
}
static void hA(void) { put('A'); }
static void hB(void) { put('B'); _exit(33); }

TC_CASE(exit_skips_remaining)
{
    DWORD code = 99; HANDLE h; char buf[8]; DWORD n = 0;
    DeleteFileA(marker);
    CHECK(tc_spawn_self("regexit", &code, 20000));
    CHECK_EQ_U(code, 33);
    buf[0] = 0;
    h = CreateFileA(marker, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { ReadFile(h, buf, 7, &n, NULL); CloseHandle(h); }
    buf[n] = 0;
    CHECK_STR(buf, "B");
    DeleteFileA(marker);
}

int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        volatile int r = 0;
        r |= _crt_atexit(hA); r |= _crt_atexit(hB);
        return r ? 78 : 0;
    }
    TC_RUN(exit_skips_remaining);
    return tc_end();
}
