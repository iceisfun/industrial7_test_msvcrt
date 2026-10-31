/* TC
 * id:    ucr_crt_atexit_lifo_child
 * area:  ucrt_only
 * funcs: _crt_atexit
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.22.4.2 (atexit LIFO) by analogy; _crt_atexit is an undocumented UCRT-internal registration, so LIFO is assumed
 * note:  PROBES that _crt_atexit handlers run LIFO on return from main. Child registers three handlers that append A, B, C to a marker file via kernel32; the parent expects "CBA".
 */
#include "tc.h"
#include <process.h>
#include <corecrt_startup.h>

static const char *marker = "tc_ucr_crt_atexit_lifo_child.tmp";

static void put(char ch)
{
    HANDLE h = CreateFileA(marker, FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD n;
    if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, &ch, 1, &n, NULL); CloseHandle(h);
}
static void hA(void) { put('A'); }
static void hB(void) { put('B'); }
static void hC(void) { put('C'); }

static int slurp(char *out, int cap)
{
    DWORD n = 0; HANDLE h = CreateFileA(marker, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) { out[0] = 0; return 0; }
    ReadFile(h, out, cap - 1, &n, NULL); CloseHandle(h); out[n] = 0; return 1;
}

TC_CASE(order_cba)
{
    DWORD code = 99; char buf[32];
    DeleteFileA(marker);
    CHECK(tc_spawn_self("reg3", &code, 20000));
    CHECK_EQ_U(code, 0);          /* child returns 0 only if every _crt_atexit returned 0 */
    CHECK(slurp(buf, sizeof buf));
    CHECK_STR(buf, "CBA");
    DeleteFileA(marker);
}

int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        volatile int r = 0;
        r |= _crt_atexit(hA); r |= _crt_atexit(hB); r |= _crt_atexit(hC);
        return r ? 78 : 0;
    }
    TC_RUN(order_cba);
    return tc_end();
}
