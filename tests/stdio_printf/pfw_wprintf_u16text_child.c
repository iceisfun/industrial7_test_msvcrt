/* TC
 * id:    pfw_wprintf_u16text_child
 * area:  stdio_printf
 * funcs: wprintf _setmode _fileno _wfreopen
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _setmode (_O_U16TEXT), wprintf
 * note:  Child reopens its stdout onto a temp file, sets _O_U16TEXT and calls wprintf; the parent reads the bytes with ReadFile. An optional BOM is tolerated, not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <wchar.h>

static int slurp(const char *fn, unsigned char *buf, int cap)
{
    DWORD n = 0; HANDLE h = CreateFileA(fn, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, buf, cap, &n, NULL)) n = (DWORD)-1;
    CloseHandle(h); return (int)n;
}
static int bom_len(const unsigned char *b, int n) { return (n >= 2 && b[0] == 0xFF && b[1] == 0xFE) ? 2 : 0; }
#ifndef _O_U16TEXT
#define _O_U16TEXT 0x20000
#endif
#ifndef _O_U8TEXT
#define _O_U8TEXT 0x40000
#endif
#ifndef _O_WTEXT
#define _O_WTEXT 0x10000
#endif
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define FN "tc_pfw_u16_" TC_S1(TC_CRT_VER) ".tmp"
static DWORD code = 999; static int ran;
static int child(void)
{
    int n;
    if (!_wfreopen(L"" FN, L"w", stdout)) return 51;
    _setmode(_fileno(stdout), _O_U16TEXT);
    n = wprintf(L"A%ls\n", L"\x4E2D");
    fflush(stdout);
    return n == 3 ? 0 : 50;
}
TC_CASE(child_reopens_stdout_to_tmp_setmode_wprintf_payload_is_utf16le)
{
    unsigned char b[64]; int n, o;
    ran = tc_spawn_self("u16", &code, 15000); CHECK(ran);
    n = slurp(FN, b, 64); CHECK(n > 0); if (n <= 0) return;
    o = bom_len(b, n); CHECK_EQ_I(n - o, 8);
    CHECK(b[o] == 'A' && b[o+1] == 0 && b[o+2] == 0x2D && b[o+3] == 0x4E && b[o+4] == 0x0D && b[o+5] == 0 && b[o+6] == 0x0A && b[o+7] == 0);
}
TC_CASE(child_exit_0) { CHECK(ran); CHECK_EQ_U(code, 0); }
TC_CASE(return_count_matches) { CHECK(ran); CHECK(code != 50); CHECK(code != 51); }
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name())) return child();
    TC_RUN(child_reopens_stdout_to_tmp_setmode_wprintf_payload_is_utf16le); TC_RUN(child_exit_0); TC_RUN(return_count_matches);
    DeleteFileA(FN);
    return tc_end();
}
