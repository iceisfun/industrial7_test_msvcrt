/* TC
 * id:    ucr_crt_atexit_nested_child
 * area:  ucrt_only
 * funcs: _crt_atexit
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.22.4.2 does not address registration during exit processing; MS UCRT behaviour
 * note:  PROBES that a handler registered from inside an exit handler is also run. Child registers A, then B; B registers D.
 *        Expected order: B, D, A (the newly registered handler is the most recent registration, so it runs next).
 */
#include "tc.h"
#include <process.h>
#include <corecrt_startup.h>

static const char *marker = "tc_ucr_crt_atexit_nested_child.tmp";

static void put(char ch)
{
    HANDLE h = CreateFileA(marker, FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD n;
    if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, &ch, 1, &n, NULL); CloseHandle(h);
}
static void hA(void) { put('A'); }
static void hD(void) { put('D'); }
static void hB(void) { put('B'); _crt_atexit(hD); }

TC_CASE(nested)
{
    DWORD code = 99; char buf[32]; DWORD n = 0; HANDLE h;
    DeleteFileA(marker);
    CHECK(tc_spawn_self("nest", &code, 20000));
    CHECK_EQ_U(code, 0);
    buf[0] = 0;
    h = CreateFileA(marker, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { ReadFile(h, buf, 31, &n, NULL); CloseHandle(h); buf[n] = 0; }
    CHECK_STR(buf, "BDA");
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
    TC_RUN(nested);
    return tc_end();
}
