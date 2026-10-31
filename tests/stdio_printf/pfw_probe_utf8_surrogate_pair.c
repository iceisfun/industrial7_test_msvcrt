/* TC
 * id:    pfw_probe_utf8_surrogate_pair
 * area:  stdio_printf
 * funcs: fwprintf wprintf _wfopen _setmode _wfreopen
 * crt:   >=100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fopen (ccs=UTF-8), _setmode (_O_U8TEXT)
 * note:  Probing: a UTF-16 surrogate pair D83D DE00 printed with %ls to a UTF-8 text stream is encoded as ONE 4-byte sequence F0 9F 98 80
 *        (not two 3-byte CESU-style sequences or '?'). Split from the documented UTF-8 tests because older CRTs may convert per code unit.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <wchar.h>

#ifndef _O_U8TEXT
#define _O_U8TEXT 0x40000
#endif
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define FN "tc_pfw_u8sp_" TC_S1(TC_CRT_VER) ".tmp"
static int slurp(unsigned char *buf, int cap)
{
    DWORD n = 0; HANDLE h = CreateFileA(FN, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, buf, cap, &n, NULL)) n = (DWORD)-1;
    CloseHandle(h); return (int)n;
}
static int child(void)
{
    wchar_t pair[3] = { 0xD83D, 0xDE00, 0 };
    if (!_wfreopen(L"" FN, L"w", stdout)) return 51;
    _setmode(_fileno(stdout), _O_U8TEXT);
    wprintf(L"%ls", pair); fflush(stdout);
    return 0;
}
TC_CASE(ccs_utf8_file_pair_is_4_bytes)
{
    unsigned char b[64]; wchar_t pair[3] = { 0xD83D, 0xDE00, 0 }; FILE *f = _wfopen(L"" FN, L"w, ccs=UTF-8"); int n;
    CHECK(f != NULL); if (!f) return;
    fwprintf(f, L"%ls", pair); fclose(f);
    n = slurp(b, 64); CHECK_EQ_I(n, 7);
    CHECK(b[3] == 0xF0 && b[4] == 0x9F && b[5] == 0x98 && b[6] == 0x80);
    DeleteFileA(FN);
}
TC_CASE(u8text_stdout_pair_is_4_bytes)
{
    unsigned char b[64]; DWORD code = 999; int n, o = 0;
    DeleteFileA(FN);
    CHECK(tc_spawn_self("p", &code, 15000)); CHECK_EQ_U(code, 0);
    n = slurp(b, 64); DeleteFileA(FN);
    if (n >= 3 && b[0] == 0xEF && b[1] == 0xBB && b[2] == 0xBF) o = 3;
    CHECK_EQ_I(n - o, 4); if (n - o != 4) return;
    CHECK(b[o] == 0xF0 && b[o+1] == 0x9F && b[o+2] == 0x98 && b[o+3] == 0x80);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) return child();
    TC_RUN(ccs_utf8_file_pair_is_4_bytes); TC_RUN(u8text_stdout_pair_is_4_bytes);
    return tc_end();
}
