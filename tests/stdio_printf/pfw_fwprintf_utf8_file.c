/* TC
 * id:    pfw_fwprintf_utf8_file
 * area:  stdio_printf
 * funcs: fwprintf _wfopen
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen, _wfopen (ccs=UTF-8: BOM EF BB BF written for a new file, data encoded as UTF-8)
 * note:  Read back with kernel32 ReadFile.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define FN "tc_pfw_utf8_" TC_S1(TC_CRT_VER) ".tmp"
#define WFN L"" FN
static int slurp(unsigned char *buf, int cap)
{
    DWORD n = 0; HANDLE h = CreateFileA(FN, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, buf, cap, &n, NULL)) n = (DWORD)-1;
    CloseHandle(h); return (int)n;
}
TC_CASE(ascii_with_bom)
{
    unsigned char b[64]; FILE *f = _wfopen(WFN, L"w, ccs=UTF-8"); int n;
    CHECK(f != NULL); if (!f) return;
    fwprintf(f, L"Hi%d", 7); fclose(f);
    n = slurp(b, 64); CHECK_EQ_I(n, 6);
    CHECK(b[0] == 0xEF && b[1] == 0xBB && b[2] == 0xBF && b[3] == 'H' && b[4] == 'i' && b[5] == '7');
    DeleteFileA(FN);
}
TC_CASE(U_00E9_C3A9)
{
    unsigned char b[64]; FILE *f = _wfopen(WFN, L"w, ccs=UTF-8"); int n;
    CHECK(f != NULL); if (!f) return;
    fwprintf(f, L"%c", (int)0x00E9); fclose(f);
    n = slurp(b, 64); CHECK_EQ_I(n, 5); CHECK(b[3] == 0xC3 && b[4] == 0xA9);
    DeleteFileA(FN);
}
TC_CASE(U_4E2D_E4B8AD)
{
    unsigned char b[64]; FILE *f = _wfopen(WFN, L"w, ccs=UTF-8"); int n;
    CHECK(f != NULL); if (!f) return;
    fwprintf(f, L"%c", (int)0x4E2D); fclose(f);
    n = slurp(b, 64); CHECK_EQ_I(n, 6); CHECK(b[3] == 0xE4 && b[4] == 0xB8 && b[5] == 0xAD);
    DeleteFileA(FN);
}
TC_CASE(crlf)
{
    unsigned char b[64]; FILE *f = _wfopen(WFN, L"w, ccs=UTF-8"); int n;
    CHECK(f != NULL); if (!f) return;
    fwprintf(f, L"a\nb"); fclose(f);
    n = slurp(b, 64); CHECK_EQ_I(n, 7);
    CHECK(b[3] == 'a' && b[4] == 0x0D && b[5] == 0x0A && b[6] == 'b');
    DeleteFileA(FN);
}
int main(void)
{
    tc_begin();
    TC_RUN(ascii_with_bom); TC_RUN(U_00E9_C3A9); TC_RUN(U_4E2D_E4B8AD); TC_RUN(crlf);
    return tc_end();
}
