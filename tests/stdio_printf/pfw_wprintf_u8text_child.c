/* TC
 * id:    pfw_wprintf_u8text_child
 * area:  stdio_printf
 * funcs: wprintf _setmode _wfreopen
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _setmode (_O_U8TEXT), wprintf
 * note:  Child process; stdout reopened to a temp file. A BOM is deliberately not asserted either way.
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
#define FN "tc_pfw_u8_" TC_S1(TC_CRT_VER) ".tmp"
static int child(const char *name)
{
    if (!_wfreopen(L"" FN, L"w", stdout)) return 51;
    _setmode(_fileno(stdout), _O_U8TEXT);
    if (name[0] == 'a') wprintf(L"A%c", (int)0x00E9);
    else wprintf(L"%c", (int)0x4E2D);
    fflush(stdout);
    return 0;
}
static int run(const char *name, unsigned char *b)
{
    DWORD code = 999; int n;
    DeleteFileA(FN);
    CHECK(tc_spawn_self(name, &code, 15000)); CHECK_EQ_U(code, 0);
    n = slurp(FN, b, 64); DeleteFileA(FN);
    if (n < 0) return 0;
    return n;
}
TC_CASE(ascii_and_U_00E9_bytes) { unsigned char b[64]; int n = run("a", b), o = (n >= 3 && b[0]==0xEF && b[1]==0xBB && b[2]==0xBF) ? 3 : 0; CHECK_EQ_I(n - o, 3); CHECK(b[o] == 'A' && b[o+1] == 0xC3 && b[o+2] == 0xA9); }
TC_CASE(U_4E2D_bytes) { unsigned char b[64]; int n = run("z", b), o = (n >= 3 && b[0]==0xEF && b[1]==0xBB && b[2]==0xBF) ? 3 : 0; CHECK_EQ_I(n - o, 3); CHECK(b[o] == 0xE4 && b[o+1] == 0xB8 && b[o+2] == 0xAD); }
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name())) return child(c);
    TC_RUN(ascii_and_U_00E9_bytes); TC_RUN(U_4E2D_bytes);
    return tc_end();
}
