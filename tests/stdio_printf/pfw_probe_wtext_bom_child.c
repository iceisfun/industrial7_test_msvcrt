/* TC
 * id:    pfw_probe_wtext_bom_child
 * area:  stdio_printf
 * funcs: wprintf _setmode _wfreopen
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _setmode (_O_WTEXT)
 * note:  Probing: _setmode(_O_WTEXT) on an already-open handle does NOT write a BOM (BOMs are only written by fopen/_open with ccs=); file is exactly 41 00. The U16TEXT test deliberately does not assert BOM presence.
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
#define FN "tc_pfw_wt_" TC_S1(TC_CRT_VER) ".tmp"
static int child(void)
{
    if (!_wfreopen(L"" FN, L"w", stdout)) return 51;
    _setmode(_fileno(stdout), _O_WTEXT);
    wprintf(L"A"); fflush(stdout);
    return 0;
}
TC_CASE(child_wtext_setmode_writes_no_bom)
{
    unsigned char b[64]; DWORD code = 999; int n;
    DeleteFileA(FN);
    CHECK(tc_spawn_self("w", &code, 15000)); CHECK_EQ_U(code, 0);
    n = slurp(FN, b, 64); DeleteFileA(FN);
    CHECK_EQ_I(n, 2); if (n < 2) return;
    CHECK(b[0] == 'A' && b[1] == 0);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) return child();
    TC_RUN(child_wtext_setmode_writes_no_bom); return tc_end();
}
