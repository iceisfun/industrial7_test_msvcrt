/* TC
 * id:    pfw_fwprintf_utf16le_file
 * area:  stdio_printf
 * funcs: fwprintf _wfopen fputws
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen, _wfopen (ccs=UTF-16LE writes a BOM and UTF-16LE data); fwprintf
 * note:  Output is read back with kernel32 ReadFile, not the CRT. Text mode: \n becomes CR LF (0D 00 0A 00).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define FN "tc_pfw_utf16le_" TC_S1(TC_CRT_VER) ".tmp"
#define WFN L"" FN
static int slurp(unsigned char *buf, int cap)
{
    DWORD n = 0; HANDLE h = CreateFileA(FN, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, buf, cap, &n, NULL)) n = (DWORD)-1;
    CloseHandle(h); return (int)n;
}
static const wchar_t *volatile vfmt;
TC_CASE(bom_FFFE_ascii_text)
{
    unsigned char b[64]; FILE *f = _wfopen(WFN, L"w, ccs=UTF-16LE"); int n;
    CHECK(f != NULL); if (!f) return;
    fwprintf(f, L"Hi"); fclose(f);
    n = slurp(b, 64); CHECK_EQ_I(n, 6);
    CHECK(b[0] == 0xFF && b[1] == 0xFE && b[2] == 'H' && b[3] == 0 && b[4] == 'i' && b[5] == 0);
    DeleteFileA(FN);
}
TC_CASE(bmp_char_bytes)
{
    unsigned char b[64]; FILE *f = _wfopen(WFN, L"w, ccs=UTF-16LE"); int n;
    CHECK(f != NULL); if (!f) return;
    fwprintf(f, L"%c%c", (int)0x00E9, (int)0x4E2D); fclose(f);
    n = slurp(b, 64); CHECK_EQ_I(n, 6);
    CHECK(b[2] == 0xE9 && b[3] == 0x00 && b[4] == 0x2D && b[5] == 0x4E);
    DeleteFileA(FN);
}
TC_CASE(newline_becomes_0D00_0A00)
{
    unsigned char b[64]; FILE *f = _wfopen(WFN, L"w, ccs=UTF-16LE"); int n;
    CHECK(f != NULL); if (!f) return;
    fwprintf(f, L"a\nb"); fclose(f);
    n = slurp(b, 64); CHECK_EQ_I(n, 2 + 10);
    CHECK(b[2] == 'a' && b[3] == 0 && b[4] == 0x0D && b[5] == 0 && b[6] == 0x0A && b[7] == 0 && b[8] == 'b' && b[9] == 0);
    DeleteFileA(FN);
}
TC_CASE(return_count_wchars)
{
    FILE *f = _wfopen(WFN, L"w, ccs=UTF-16LE"); int n, m;
    CHECK(f != NULL); if (!f) return;
    n = fwprintf(f, L"abc%d", 12345); m = fputws(L"xy", f);
    CHECK_EQ_I(n, 8); CHECK(m >= 0);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(surrogate_pair_bytes)
{
    unsigned char b[64]; wchar_t pair[3] = { 0xD83D, 0xDE00, 0 }; FILE *f = _wfopen(WFN, L"w, ccs=UTF-16LE"); int n, r;
    CHECK(f != NULL); if (!f) return;
    r = fwprintf(f, L"%ls", pair); fclose(f);
    CHECK_EQ_I(r, 2);
    n = slurp(b, 64); CHECK_EQ_I(n, 6);
    CHECK(b[2] == 0x3D && b[3] == 0xD8 && b[4] == 0x00 && b[5] == 0xDE);
    DeleteFileA(FN);
}
int main(void)
{
    tc_begin(); vfmt = L"";
    TC_RUN(bom_FFFE_ascii_text); TC_RUN(bmp_char_bytes); TC_RUN(newline_becomes_0D00_0A00);
    TC_RUN(return_count_wchars); TC_RUN(surrogate_pair_bytes);
    return tc_end();
}
