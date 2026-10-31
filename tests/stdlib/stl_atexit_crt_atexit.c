/* TC
 * id:    stl_atexit_crt_atexit
 * area:  stdlib
 * funcs: _crt_atexit
 * crt:   >=140
 * tier:  p1
 * conf:  observed
 * ref:   UCRT internal export _crt_atexit (same semantics as atexit: handlers run in reverse registration order at exit)
 * note:  Child registers two handlers that append markers to tc_stl_atexit.tmp, then returns from main.
 */
#include "tc.h"

#define FN "tc_stl_atexit.tmp"
extern int __cdecl _crt_atexit(void (__cdecl *)(void));

static void mark(char c)
{
    HANDLE h = CreateFileA(FN, FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD n;
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, &c, 1, &n, NULL); CloseHandle(h); }
}
static void __cdecl hA(void) { mark('A'); }
static void __cdecl hB(void) { mark('B'); }

TC_CASE(lifo_order)
{
    DWORD code = 99; int ran; char buf[16] = {0}; DWORD n = 0; HANDLE h;
    DeleteFileA(FN);
    ran = tc_spawn_self("reg", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 0);   /* child: both registrations returned 0 */
    h = CreateFileA(FN, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    CHECK(h != INVALID_HANDLE_VALUE);
    if (h != INVALID_HANDLE_VALUE) { ReadFile(h, buf, 15, &n, NULL); CloseHandle(h); }
    buf[n] = 0;
    CHECK_STR(buf, "BA");
    DeleteFileA(FN);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        int r1 = _crt_atexit(hA);
        int r2 = _crt_atexit(hB);
        return (r1 == 0 && r2 == 0) ? 0 : 3;
    }
    TC_RUN(lifo_order);
    return tc_end();
}
