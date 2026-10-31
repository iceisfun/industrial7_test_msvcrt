/* TC
 * id:    pfw_wprintf_u16text_surrogate_child
 * area:  stdio_printf
 * funcs: wprintf vwprintf _setmode _wfreopen
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _setmode (_O_U16TEXT: UTF-16 output passes through), wprintf, vwprintf
 * note:  Child process; stdout reopened to a temp file. The parent compares the bytes after an optional BOM.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
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
#define FN "tc_pfw_u16s_" TC_S1(TC_CRT_VER) ".tmp"
static int vw(const wchar_t *fmt, ...) { va_list ap; int r; va_start(ap, fmt); r = vwprintf(fmt, ap); va_end(ap); return r; }
static int child(const char *name)
{
    wchar_t pair[3] = { 0xD83D, 0xDE00, 0 };
    if (!_wfreopen(L"" FN, L"w", stdout)) return 51;
    _setmode(_fileno(stdout), _O_U16TEXT);
    if (name[0] == 'l') wprintf(L"%ls", pair);
    else if (name[0] == 'c') wprintf(L"%c%c", (int)0xD83D, (int)0xDE00);
    else if (name[0] == 'v') vw(L"%ls", pair);
    else wprintf(L"\n");
    fflush(stdout);
    return 0;
}
static int run(const char *name, unsigned char *b, int *o)
{
    DWORD code = 999; int n;
    DeleteFileA(FN);
    CHECK(tc_spawn_self(name, &code, 15000)); CHECK_EQ_U(code, 0);
    n = slurp(FN, b, 64); DeleteFileA(FN);
    if (n < 0) n = 0;
    *o = bom_len(b, n); return n - *o;
}
static void check_pair(const char *nm)
{
    unsigned char b[64]; int o, n = run(nm, b, &o);
    CHECK_EQ_I(n, 4); CHECK(b[o] == 0x3D && b[o+1] == 0xD8 && b[o+2] == 0x00 && b[o+3] == 0xDE);
}
TC_CASE(pair_in_ls_bytes_3DD800DE) { check_pair("l"); }
TC_CASE(c_high_c_low_adjacent_units) { check_pair("c"); }
TC_CASE(vwprintf_variant) { check_pair("v"); }
TC_CASE(newline_0D000A00) { unsigned char b[64]; int o, n = run("n", b, &o); CHECK_EQ_I(n, 4); CHECK(b[o] == 0x0D && b[o+1] == 0 && b[o+2] == 0x0A && b[o+3] == 0); }
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name())) return child(c);
    TC_RUN(pair_in_ls_bytes_3DD800DE); TC_RUN(c_high_c_low_adjacent_units); TC_RUN(vwprintf_variant); TC_RUN(newline_0D000A00);
    return tc_end();
}
